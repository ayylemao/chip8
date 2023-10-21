#pragma once
#include "utils.h"
#include <cstddef> // For std::size_t

struct Memory {
    Byte data[4096];

    // Function declarations
    void init();
    Byte& operator[](u32 addr);  // This allows you to use memory[index] syntax.
    const Byte& operator[](u32 addr) const;  // For const instances of Memory
};