#include "../include/cpu.h"
#include "cpu.h"



void CPU::reset(Memory& memory)
{
    for (int i = 0; i<NREG; i++)
    {
        V[i] = 0x0;
        keyboard[i] = false;
    }
    PC = 0x200;
    SP = 0x0;
    I = 0x0;
    redraw = false;
    memory.init();
    for (int disp_x = 0; disp_x < DISPN_X; disp_x++)
    {
        for (int disp_y = 0; disp_y < DISPN_Y; disp_y++)
        {
            display[disp_y][disp_x] = false;
        }
    }
};

void CPU::loadSprites(Memory& memory)
{
    for (int byte = 0; byte < 128; byte++)
    {
        memory[byte] = characters[byte];
    }
}
    
Word CPU::fetch(Memory& memory)
{
    Byte ins1 = memory[PC];
    PC++;
    Byte ins2 = memory[PC];
    PC++;
    Word data = (static_cast<Word>(ins1) << 8 | ins2);
    return data;
};

Byte CPU::randByte()
{
    std::mt19937 gen(rd());
    std::uniform_int_distribution<Byte> distrib(0, 255);
    return distrib(gen);
}

void CPU::execute(Memory& memory)
{
    Word ins = fetch(memory);
    Word Vx = (ins >> 8) & 0x000F;
    Word Vy = (ins >> 4) & 0x000F;
    Byte xcoord;
    Byte ycoord;
    Byte carry;
    Word N;
    Byte pixelByte;
    bool pixel;
    ticks += 1;
    if ((ticks % 9 == 0) & (DT > 0))
    {
        DT -= 1;
    }

    switch (ins)
    {
        case SYS_addr:
            return;
        case CLS:
            // Clear the display.
            redraw = true;
            for (int y = 0; y < DISPN_Y; y++)
            {
                for (int x = 0; x < DISPN_X; x++)
                {
                    display[y][x] = 0;
                }
            }
            return;
        case RET:
            if (SP == 0)
            {
                std::cout << "Stack Underflow at PC ";
                printWord(PC);
                std::_Exit(EXIT_FAILURE);
            }
            SP--;
            PC = Stack[SP];
            return;
        default:
            break;
    }

    switch (ins & 0xF000)
    {
    case JP_ADDR:
        PC = ins & 0xFFF;
        break;
    case CALL_addr:
        if (SP == 15)
        {
            std::cout << "Stack Overflow at PC ";
            printWord(PC);
            std::_Exit(EXIT_FAILURE);
        }
        Stack[SP] = PC;
        SP++;
        PC = (ins & 0x0FFF);
        break;
    case SE_Vx:
    {
        if (V[Vx] == static_cast<Byte>(ins & 0x00FF))
        {
            PC++;
            PC++;
        }
        break;
    }
    case SNE_Vx:
        if (V[Vx] != static_cast<Byte>(ins & 0x00FF))
        {
            PC++;
            PC++;
        }
        break;
    case SE_VxVy:
        if (V[(ins >> 8) & 0x000F] == V[Vy])
        {
            PC++;
            PC++;
        }
        break;
    case LD_Vx:
        V[Vx] = static_cast<Byte>(ins & 0x00FF);
        break;
    case ADD_Vx:
        V[Vx] = V[Vx] + (ins & 0x00FF) % 256;
        break;
    case COMP_INS:
        switch (ins & 0xF00F)
        {
            case LD_VxVy:
                V[Vx] = V[Vy]; 
                break;
            case OR_VxVy:
                V[Vx] = V[Vx] | V[Vy];
                break; 
            case AND_VxVy:
                V[Vx] = V[Vx] & V[Vy];
                break;
            case XOR_VxVy:
                V[Vx] = V[Vx] ^ V[Vy];
                break;
            case ADD_VxVy:
                carry =  (V[Vx] + V[Vy]) > 255 ? 1 : 0;
                V[Vx] = V[Vx] + V[Vy];
                V[0xF] = carry;
                break;
            case SUB_VxVy:
                carry =  (V[Vx] >= V[Vy])  ? 1 : 0;
                V[Vx] = V[Vx] - V[Vy];
                V[0xF] = carry;

                break;
            case SHR_Vx:
                carry = V[Vx] & 0x01;
                V[Vx] >>= 1;
                V[0x0F] = carry;
                break;
            case SUBN_VxVy:
                V[Vx] = V[Vy] - V[Vx];
                V[0x0F] = V[Vy] > V[Vx] ? 1 : 0;
                break;
            case SHL_Vx:
                carry = (V[Vx] & 0x80) >> 7;
                V[Vx] = V[Vx] << 1;
                V[0x0F] = carry;
                break;
            default:
                break;
        }
        break;
    case SNE_VxVy:
        if (V[Vx] != V[Vy])
        {
            PC++;
            PC++;
        }
        break;
    case LD_I:
        I = ins & 0x0FFF;
        break;
    case JP_V0:
        PC = (ins & 0x0FFF) + V[0x0];
        break;
    case RND_Vx:
        V[Vx] = randByte() & static_cast<Byte>(ins & 0x00FF);
        break;
    case DRW_VxVy:
        redraw = true;
        xcoord = V[Vx] % DISPN_X;
        ycoord = V[Vy] % DISPN_Y;
        V[0x0F] = 0;
        N = ins & 0x000F;
        for (int row = 0; row < N; row++)
        {
            pixelByte = memory[I + row];
            for (int x = 7; x >= 0; --x)
            {
                pixel = (pixelByte & (1 << x)) != 0;
                if (pixel)
                {
                    if (display[ycoord + row][xcoord + 7 - x]) 
                    {
                        V[0xF] = 1;
                    }
                    display[(ycoord + row) % DISPN_Y][(xcoord + 7 - x) % DISPN_X] ^= true;
                }
            }
        }

        break;
    case KEY_OPS:
        switch (ins & 0xF0FF)
        {
        case SKP_Vx:
            //  Skip next instruction if key with the value of Vx is pressed.
            if (V[Vx] <= 0xF)
            {
                if (keyboard[V[Vx]])
                {
                    PC++;
                    PC++;
                }
            }
            break;
        case SKNP_Vx:
            // Skip next instruction if key with the value of Vx is not pressed. 
            if (V[Vx] <= 0xF)
            {
                if (!keyboard[V[Vx]])
                {
                    PC++;
                    PC++;
                }
            }
            break;
        default:
            break;
        }
        break;
    case DTST_OPS:
        switch (ins & 0xF0FF)
        {
        case LD_VxDT:
            // Set Vx = delay timer value.
            V[Vx] = DT;
            break;
        case LD_Vx_K:
            // Wait for a key press, store the value of the key in Vx.
            for (int i = 0; i < 16; i++)
            {
                if (keyboard[i])
                {
                    V[Vx] = i;
                    break;
                }
            }
            PC--;
            PC--;
            break;
        case LD_DTVx:
            // Set delay timer = Vx.
            DT = V[Vx];
            break;
        case LD_STVx:
            // TODO:
            // Set sound timer = Vx.
            std::cout << "Not Implemented LD_STVx\n";
            break;
        case ADD_IVx:
            // Set I = I + Vx.
            I = I + V[Vx];
            break;
        case LD_FVx:
            // TODO:
            // Set I = location of sprite for digit Vx.
            std::cout << "Not Implemented LD_FVx\n";
            break;
        case LD_BVx:
            // Store BCD representation of Vx in memory locations I, I+1, and I+2.
            memory[I] = V[Vx] / 100;
            memory[I + 1] = (V[Vx] / 10) % 10;
            memory[I + 2] = (V[Vx] % 10);
            break;
        case LD_STO_IVx:
            // Store registers V0 through Vx in memory starting at location I.
            for (int i = 0; i<= Vx; i++)
            {
                memory[I+i] = V[i];
            }
            break;
        case LD_STO_VxI:
            // Read registers V0 through Vx from memory starting at location I.
            for (int i = 0; i <= Vx; i++)
            {
                V[i] = memory[I+i];
            }
            break;
        default:
            break;
        }
        break;
    case LD_VxDT:
        V[Vx] = DT;
    default:
        break;
    }
};
