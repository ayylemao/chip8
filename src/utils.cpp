#include "../include/utils.h"   
#include "utils.h"



void utils::printByte(Byte value)
{
    std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u32>(value) << '\n';
}

void utils::printOpcode(Byte value1, Byte value2)
{
    std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u32>(value1);
    std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<u32>(value2) << '\t';
}
void utils::printWord(Word value)
{
    std::cout << "0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u32>(value) << '\n';
}

std::vector<Byte> utils::loadROM(const std::string& filename) 
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open ROM file " << filename << std::endl;
        return {}; // Return an empty vector in case of failure
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<Byte> romContents(size);
    if (!file.read(reinterpret_cast<char*>(romContents.data()), size)) {
        std::cerr << "Error: Could not read ROM file" << std::endl;
        return {}; // Return an empty vector in case of failure
    }

    file.close();
    return romContents;
}
