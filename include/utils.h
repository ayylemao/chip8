#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <fstream>
#include <vector>
#include <cstdint>
#include "../include/memory.h"

using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

namespace utils
{
    void printByte(Byte value);
    void printWord(Word value);
    std::vector<Byte> loadROM(const std::string& filename);
}
