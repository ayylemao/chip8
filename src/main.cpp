#include <cstdio>
//#include "../include7"
#include "../include/cpu.h"

using namespace utils;

int main()
{
    Memory memory;
    CPU cpu;
    cpu.reset(memory);
    memory.data[cpu.PC] = 0x7A;
    memory.data[cpu.PC+1] = 0x05;
    cpu.V[0xA] = 0x01;
    cpu.execute(memory);

}