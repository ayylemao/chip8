#include "../include/cpu.h"



void CPU::reset(Memory& memory)
{
    for (int i = 0; i<NREG; i++)
    {
        V[i] = 0x0;
    }
    PC = 0x200;
    SP = 0x0;
    memory.init();
};
    
Word CPU::fetch(Memory& memory)
{
    Byte ins1 = memory[PC];
    PC++;
    Byte ins2 = memory[PC];
    PC++;
    Word data = (static_cast<Word>(ins1) << 8 | ins2);
    return data;
};

void CPU::execute(Memory& memory)
{
    Word ins = fetch(memory);
    Word Vx;
    Word Vy;
    switch (ins & 0xF000)
    {
    case JP_ADDR:
        PC = ins & 0xFFF;
        break;
    case SE_Vx:
    {
        Word Vx = (ins >> 8) & 0x000F;
        if (V[Vx] == static_cast<Byte>(ins & 0x00FF))
        {
            PC++;
            PC++;
        }
        break;
    }
    case SNE_Vx:
        Vx = (ins >> 8) & 0x000F;
        if (V[Vx] != static_cast<Byte>(ins & 0x00FF))
        {
            PC++;
            PC++;
        }
        break;
    case SE_VxVy:
        Vx = (ins >> 8) & 0x000F;
        Vy = (ins >> 4) & 0x000F;
        if (V[(ins >> 8) & 0x000F] == V[Vy])
        {
            PC++;
            PC++;
        }
        break;
    case LD_Vx:
        Vx = (ins >> 8) & 0x000F;
        V[Vx] = static_cast<Byte>(ins & 0x00FF);
        break;
    case ADD_Vx:
        Vx = (ins >> 8) & 0x000F;
        V[Vx] = V[Vx] + (ins & 0x00FF) % 256;
        break;
    case COMP_INS:
        Vx = (ins >> 8) & 0x000F;
        Vy = (ins >> 4) & 0x000F;
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
            default:
                break;
        }
    

    default:
        break;
    }
};
