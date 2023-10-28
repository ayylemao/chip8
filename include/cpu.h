#pragma once
#include <random>
#include "../include/utils.h"
#include "../include/memory.h"

using namespace utils;
struct CPU
{
    static constexpr int NREG = 16;
    static constexpr int SMEM = 4096;
    Byte V[16];
    Word I;
    Word PC;
    Byte SP;
    void reset(Memory& memory);
    Word fetch(Memory& memory);
    void execute(Memory& memory);
    std::random_device rd;
    Byte randByte();

    // OPCODES:
    static constexpr Word JP_ADDR = 0x1000;
    static constexpr Word SE_Vx = 0x3000;
    static constexpr Word SNE_Vx = 0x4000;
    static constexpr Word SE_VxVy = 0x5000;
    static constexpr Word LD_Vx = 0x6000;
    static constexpr Word ADD_Vx = 0x7000;
    static constexpr Word COMP_INS = 0x8000;
    static constexpr Word LD_VxVy = 0x8000;
    static constexpr Word OR_VxVy = 0x8001;
    static constexpr Word AND_VxVy = 0x8002;
    static constexpr Word XOR_VxVy = 0x8003;
    static constexpr Word ADD_VxVy = 0x8004;
    static constexpr Word SUB_VxVy = 0x8005;
    static constexpr Word SHR_Vx = 0x8006;
    static constexpr Word SUBN_VxVy = 0x8007;
    static constexpr Word SHL_Vx = 0x800E;
    static constexpr Word SNE_VxVy = 0x9000;
    static constexpr Word LD_I = 0xA000;
    static constexpr Word JP_V0 = 0xB000;
    static constexpr Word RND_Vx = 0xC000;
};