#pragma once
#include <random>
#include "../include/utils.h"
#include "../include/memory.h"

using namespace utils;
struct CPU
{
    static constexpr int NREG = 16;
    static constexpr int SMEM = 4096;
    static constexpr int DISPN_X = 64;
    static constexpr int DISPN_Y = 32;
    Byte V[16];
    Word Stack[16];
    bool display[DISPN_Y][DISPN_X];
    Word I;
    Word PC;
    Byte SP;
    Byte DT;
    Byte ST;
    void reset(Memory& memory);
    Word fetch(Memory& memory);
    void execute(Memory& memory);
    std::random_device rd;
    Byte randByte();
    void loadSprites(Memory& memory);

    // OPCODES:
    static constexpr Word SYS_addr = 0x0000;
    static constexpr Word CLS = 0x00E0;
    static constexpr Word RET = 0x00EE;
    static constexpr Word JP_ADDR = 0x1000;
    static constexpr Word CALL_addr = 0x2000;
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
    static constexpr Word DRW_VxVy = 0xD000;
    static constexpr Word KEY_OPS = 0xE000;
    static constexpr Word SKP_Vx = 0xE093;
    static constexpr Word SKNP_Vx = 0xE0A1;
    static constexpr Word DTST_OPS = 0xF000;
    static constexpr Word LD_VxDT = 0xF007;
    static constexpr Word LD_Vx_K = 0xF00A;
    static constexpr Word LD_DTVx = 0xF015;
    static constexpr Word LD_STVx = 0xF018;
    static constexpr Word ADD_IVx = 0xF01E;
    static constexpr Word LD_FVx = 0xF029;
    static constexpr Word LD_BVx = 0xF033;
    static constexpr Word LD_STO_IVx = 0xF055;
    static constexpr Word LD_STO_VxI = 0xF065;

    // DEFAULT SPRITES:
    static constexpr Byte characters[16][8] =
    {
        {0xF0, 0x90, 0x90, 0x90, 0xF0, 0x0, 0x0, 0x0},  // 0
        {0x20, 0x60, 0x20, 0x20, 0x70, 0x0, 0x0, 0x0},  // 1
        {0xF0, 0x10, 0xF0, 0x80, 0xF0, 0x0, 0x0, 0x0},  // 2
        {0xF0, 0x10, 0xF0, 0x10, 0xF0, 0x0, 0x0, 0x0},  // 3
        {0x90, 0x90, 0xF0, 0x10, 0x10, 0x0, 0x0, 0x0},  // 4
        {0xF0, 0x80, 0xF0, 0x10, 0xF0, 0x0, 0x0, 0x0},  // 5
        {0xF0, 0x80, 0xF0, 0x90, 0xF0, 0x0, 0x0, 0x0},  // 6
        {0xF0, 0x10, 0x20, 0x40, 0x40, 0x0, 0x0, 0x0},  // 7
        {0xF0, 0x90, 0xF0, 0x90, 0xF0, 0x0, 0x0, 0x0},  // 8
        {0xF0, 0x90, 0xF0, 0x10, 0xF0, 0x0, 0x0, 0x0},  // 9
        {0xF0, 0x90, 0xF0, 0x90, 0x90, 0x0, 0x0, 0x0},  // A
        {0xE0, 0x90, 0xE0, 0x90, 0xE0, 0x0, 0x0, 0x0},  // B
        {0xF0, 0x80, 0x80, 0x80, 0xF0, 0x0, 0x0, 0x0},  // C
        {0xE0, 0x90, 0x90, 0x90, 0xE0, 0x0, 0x0, 0x0},  // D
        {0xF0, 0x80, 0xF0, 0x80, 0xF0, 0x0, 0x0, 0x0},  // E
        {0xF0, 0x80, 0xF0, 0x80, 0x80, 0x0, 0x0, 0x0}   // F
    };
};
