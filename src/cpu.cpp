#include "../include/cpu.h"
#include "cpu.h"



void CPU::reset(Memory& memory)
{
    for (int i = 0; i<NREG; i++)
    {
        V[i] = 0x0;
    }
    PC = 0x200;
    SP = 0x0;
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
    Word N;
    Byte pixelByte;
    bool pixel;

    switch (ins)
    {
        case SYS_addr:
            return;
        case CLS:
            // TODO
            // Clear the display.
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
                V[0x0F] = V[Vx] + V[Vy] > 0xFF ? 1 : 0;
                V[Vx] = (V[Vx] + V[Vy]);
                break;
            case SUB_VxVy:
                V[0xF] = V[Vx] > V[Vy] ? 1 : 0;
                V[Vx] = V[Vx] - V[Vy];
                break;
            case SHR_Vx:
                V[0x0F] = V[Vx] & 0x01;
                V[Vx] >>= 1;
                break;
            case SUBN_VxVy:
                V[0x0F] = V[Vy] > V[Vx] ? 1 : 0;
                V[Vx] = V[Vy] - V[Vx];
                break;
            case SHL_Vx:
                V[0x0F] = (V[Vx] & 0x80) >> 7;
                V[Vx] = V[Vx] << 1;
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
        xcoord = V[Vx] % DISPN_X;
        ycoord = V[Vy] % DISPN_Y;
        V[0x0F] = 0;
        N = ins & 0x000F;
        for (int row = 0; row < N; row++)
        {
            if (ycoord + row >= DISPN_Y)
            {
                break;
            }
            pixelByte = memory[I + row];
            for (int x = 7; x >= 0; --x)
            {
                pixel = (row & (1 << i)) != 0;
                if (xcoord + x >= DISPN_X)
                {
                    break;
                }

                if ((pixel == true) & (display[ycoord + row][xcoord + x] == true))
                {
                    V[0xF] = 1;
                }
                else if ((pixel == true) & (display[ycoord + row][xcoord + x] == false))
                {
                    display[ycoord + row][xcoord + x] = true;
                }
            }
        }

        // TODO:
        // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
        break;
    case KEY_OPS:
        switch (ins & 0xF00F)
        {
        case SKP_Vx:
            // TODO:
            //  Skip next instruction if key with the value of Vx is pressed.
            break;
        case SKNP_Vx:
            // TODO:
            // Skip next instruction if key with the value of Vx is not pressed. 
            break;
        default:
            break;
        }
        break;
    case DTST_OPS:
        switch (ins & 0xF0FF)
        {
        case LD_VxDT:
            // TODO:
            // Set Vx = delay timer value.
            break;
        case LD_Vx_K:
            // TODO:
            // Wait for a key press, store the value of the key in Vx.
            break;
        case LD_DTVx:
            // TODO:
            // Set delay timer = Vx.
            break;
        case LD_STVx:
            // TODO:
            // Set sound timer = Vx.
            break;
        case ADD_IVx:
            // TODO:
            // Set I = I + Vx.
            break;
        case LD_FVx:
            // TODO:
            // Set I = location of sprite for digit Vx.
            break;
        case LD_BVx:
            // TODO:
            // Store BCD representation of Vx in memory locations I, I+1, and I+2.
            break;
        case LD_STO_IVx:
            // TODO:
            // Store registers V0 through Vx in memory starting at location I.
            break;
        case LD_STO_VxI:
            // TODO:
            // Read registers V0 through Vx from memory starting at location I.
            break;
        default:
            break;
        }

    case LD_VxDT:
        V[Vx] = DT;
    default:
        break;
    }
};
