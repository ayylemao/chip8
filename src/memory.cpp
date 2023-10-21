#include "../include/memory.h"

void Memory::init() {
    for (std::size_t i = 0; i < 4096; ++i) {
        data[i] = 0;
    }
}

Byte& Memory::operator[](u32 addr) {
    return data[addr];
}

const Byte& Memory::operator[](u32 addr) const {
    return data[addr];
}