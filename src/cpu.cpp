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

    switch (ins & 0xF000)
    {
    case JP_ADDR:
        PC = ins & 0xFFF;
        break;
    case SE_Vx:
        if (V[(ins >> 8) & 0x000F] == static_cast<Byte>(ins & 0x00FF))
        {
            PC++;
            PC++;
        }
        break;
    case SNE_Vx:
        if (V[(ins >> 8) & 0x000F] != static_cast<Byte>(ins & 0x00FF))
        {
            PC++;
            PC++;
        }
        break;
    case SE_VxVy:
        if (V[(ins >> 8) & 0x000F] == V[(ins >> 4) & 0x000F])
        {
            PC++;
            PC++;
        }
        break;
    case LD_Vx:
        V[(ins >> 8) & 0x000F] = static_cast<Byte>(ins & 0x00FF);
        break;
    case ADD_Vx:
        V[(ins >> 8) & 0x000F] = V[(ins >> 8) & 0x000F] + (ins & 0x00FF) % 256;
        break;
    case COMP_INS:
        switch (ins & 0xF00F)
        {
            case LD_VxVy:
                V[(ins >> 8) & 0x000F] = V[(ins >> 4) & 0x000F]; 
                break;
            case OR_VxVy:
                V[(ins >> 8) & 0x000F] = V[(ins >> 8) & 0x000F] | V[(ins >> 4) & 0x000F];
                break; 
            case AND_VxVy:
                V[(ins >> 8) & 0x000f] = V[(ins >> 8) & 0x000F] & V[(ins >> 4) & 0x000F];
                break;
            case XOR_VxVy:
                V[(ins >> 8) & 0x000F] = V[(ins >> 8) & 0x000F] ^ V[(ins >> 4) & 0x000F];
                break;
            case ADD_VxVy:
                if (V[(ins >> 8) & 0x000F] + V[(ins >> 4) & 0x000F] > 0xFF) {V[0xF] = 0x1;}
                V[(ins >> 8) & 0x000F] = (V[(ins >> 8) & 0x000F] + V[(ins >> 4) & 0x000F]) % 256;
                break;
            case SUB_VxVy:
                if (V[(ins >> 8) & 0x000F] > V[(ins >> 4) & 0x000F]) 
                {
                    V[0xF] = 0x1;
                    V[(ins >> 8) & 0x000F] = V[(ins >> 8) & 0x000F] - V[(ins >> 4) & 0x000F];
                }
                else if (V[(ins >> 8) & 0x000F] == V[(ins >> 4) & 0x000F])
                {
                    V[0xF] = 0;
                    V[(ins >> 8) & 0x000F] = 0x0;
                }
                else
                {
                    V[0xF] = 0x0;
                    V[(ins >> 8) & 0x000F] = (V[(ins >> 8) & 0x000F] - V[(ins >> 4) & 0x000F]) + 256;
                }
                break;
            case SHR_Vx:
                if ((V[(ins >> 8) & 0x000F]) & 0x000F == 0x0001)
                {
                    V[0x0F] = 0x1;
                   // TODO: DIVIDE BY TWO 
                }
            default:
                break;
        }
    

    default:
        break;
    }
};
