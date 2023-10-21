#include "../include/utils.h"   



void utils::printByte(Byte value)
{
    std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u32>(value) << '\n';
}

void utils::printWord(Word value)
{
    std::cout << "0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u32>(value) << '\n';
}