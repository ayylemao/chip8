#include <gtest/gtest.h>
#include "cpu.h"     // Adjust the path if needed
#include "memory.h"  // Adjust the path if needed
#include "utils.h"
#include <iostream>

using namespace utils;

class Chip8Test : public ::testing::Test {
protected:
    // This function runs before each TEST_F
    void SetUp() override {
        // Initialize things if needed
        cpu.reset(memory);
    }

    // This function runs after each TEST_F
    void TearDown() override {
        // Clean up if needed
    }

    CPU cpu;
    Memory memory;
};
TEST_F(Chip8Test, Fetch) {
    memory.data[cpu.PC] = 0x1A;
    memory.data[cpu.PC + 1] = 0xFA;
    Word fetched = cpu.fetch(memory);
    EXPECT_EQ(fetched, 0x1AFA);
}

TEST_F(Chip8Test, JMPADDR) {
    memory.data[cpu.PC] = 0x1F;
    memory.data[cpu.PC+1] = 0x4C;
    cpu.execute(memory);
    EXPECT_EQ(cpu.PC, 0x0F4C);
}

TEST_F(Chip8Test, SE_Vx) {
    Word initial_PC = cpu.PC;
    memory.data[cpu.PC] = 0x3B;
    memory.data[cpu.PC+1] = 0x4C;
    cpu.V[0xB] = 0x4C;
    cpu.execute(memory);
    EXPECT_EQ(cpu.PC, initial_PC+4);
}
TEST_F(Chip8Test, SNE_Vx) {
    Word initial_PC = cpu.PC;
    memory.data[cpu.PC] = 0x4B;
    memory.data[cpu.PC+1] = 0x8C;
    cpu.V[0xB] = 0x4B;
    cpu.execute(memory);
    EXPECT_EQ(cpu.PC, initial_PC+4);
}

TEST_F(Chip8Test, SE_VxVy) {
    Word initial_PC = cpu.PC;
    memory.data[cpu.PC] = 0x5B;
    memory.data[cpu.PC+1] = 0xC0;
    cpu.V[0xB] = 0x4B;
    cpu.V[0xC] = 0x4B;
    cpu.execute(memory);
    EXPECT_EQ(cpu.PC, initial_PC+4);
}

TEST_F(Chip8Test, LD_Vx) {
    memory.data[cpu.PC] = 0x6E;
    memory.data[cpu.PC+1] = 0xC0;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], 0xC0);
}

TEST_F(Chip8Test, ADD_Vx) {
    Byte old_value = 0xAB;
    Byte add_value = 0x05;
    memory.data[cpu.PC] = 0x7A;
    memory.data[cpu.PC+1] = add_value;
    cpu.V[0xA] = old_value;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0xA], old_value + add_value);
    cpu.reset(memory);
    old_value = 0xFA;
    add_value = 0xEC;
    memory.data[cpu.PC] = 0x7A;
    memory.data[cpu.PC+1] = add_value;
    cpu.V[0xA] = old_value;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0xA], 0xE6);
}

TEST_F(Chip8Test, LD_VxVy) {
    memory.data[cpu.PC] = 0x8E;
    memory.data[cpu.PC+1] = 0xC0;
    cpu.V[0x0E] = 0x0;
    cpu.V[0x0C] = 0xAA;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], cpu.V[0x0C]);
}
TEST_F(Chip8Test, OR_VxVy) {
    memory.data[cpu.PC] = 0x8E;
    memory.data[cpu.PC+1] = 0xC1;
    cpu.V[0x0E] = 0xCA;
    cpu.V[0x0C] = 0xAA;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], 0xEA);
}

TEST_F(Chip8Test, AND_VxVy) {
    memory.data[cpu.PC] = 0x8E;
    memory.data[cpu.PC+1] = 0xC2;
    cpu.V[0x0E] = 0xCA;
    cpu.V[0x0C] = 0xAA;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], 0x8A);
}

TEST_F(Chip8Test, XOR_VxVy) {
    memory.data[cpu.PC] = 0x8E;
    memory.data[cpu.PC+1] = 0xC3;
    cpu.V[0x0E] = 0xCA;
    cpu.V[0x0C] = 0xAA;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], 0x60);
}

TEST_F(Chip8Test, ADD_VxVy_NOF) {
    Byte RegA = 0xCA;
    Byte RegB = 0x12;
    memory.data[cpu.PC] = 0x8E;
    memory.data[cpu.PC+1] = 0xC4;
    cpu.V[0x0E] = RegA;
    cpu.V[0x0C] = RegB;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], RegA + RegB);
    EXPECT_EQ(cpu.V[0x0F], 0x0);

    cpu.reset(memory);
    RegA = 0xFA;
    RegB = 0xEF;
    memory.data[cpu.PC] = 0x8E;
    memory.data[cpu.PC+1] = 0xC4;
    cpu.V[0x0E] = RegA;
    cpu.V[0x0C] = RegB;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], 0xE9);
    EXPECT_EQ(cpu.V[0x0F], 0x1);
}

TEST_F(Chip8Test, SUB_VxVy) {
    Byte RegA = 0xCA;
    Byte RegB = 0x12;
    memory.data[cpu.PC] = 0x8E;
    memory.data[cpu.PC+1] = 0xC5;
    cpu.V[0x0E] = RegA;
    cpu.V[0x0C] = RegB;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], RegA - RegB);
    EXPECT_EQ(cpu.V[0x0F], 0x1);

    RegA = 0xCA;
    RegB = 0xCA;
    memory.data[cpu.PC] = 0x8E;
    memory.data[cpu.PC+1] = 0xC5;
    cpu.V[0x0E] = RegA;
    cpu.V[0x0C] = RegB;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], RegA - RegB);
    EXPECT_EQ(cpu.V[0x0F], 0x0);

    RegA = 0xA9;
    RegB = 0xCA;
    memory.data[cpu.PC] = 0x8E;
    memory.data[cpu.PC+1] = 0xC5;
    cpu.V[0x0E] = RegA;
    cpu.V[0x0C] = RegB;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], 0xDF);
    EXPECT_EQ(cpu.V[0x0F], 0x0);
}


TEST_F(Chip8Test, SHR_Vx) {
    Byte RegA = 0x02;
    memory.data[cpu.PC] = 0x8E;
    memory.data[cpu.PC+1] = 0xC6;
    cpu.V[0x0E] = RegA;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], 0x01);
    EXPECT_EQ(cpu.V[0x0F], 0x0);

    cpu.reset(memory);
    memory.data[cpu.PC] = 0x8E;
    memory.data[cpu.PC+1] = 0xC6;
    RegA = 0xA5;
    cpu.V[0x0E] = RegA;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0E], 0x52);
    EXPECT_EQ(cpu.V[0x0F], 0x01);
}

TEST_F(Chip8Test, SUBN_VxVy) {
    Byte RegA = 0x0A;
    Byte RegB = 0xAC;
    memory.data[cpu.PC] = 0x8A;
    memory.data[cpu.PC+1] = 0xB7;
    cpu.V[0x0A] = RegA;
    cpu.V[0x0B] = RegB;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0A], 172 - 10);
    EXPECT_EQ(cpu.V[0x0F], 0x1);

    cpu.reset(memory);
    RegA = 0xAC;
    RegB = 0x0A;
    memory.data[cpu.PC] = 0x8A;
    memory.data[cpu.PC+1] = 0xB7;
    cpu.V[0x0A] = RegA;
    cpu.V[0x0B] = RegB;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0A], 10 - 172 + 256);
    EXPECT_EQ(cpu.V[0x0F], 0x0);
}

TEST_F(Chip8Test, SHL_Vx) {
    Byte RegA = 0xAB;
    memory.data[cpu.PC] = 0x8A;
    memory.data[cpu.PC+1] = 0xBE;
    cpu.V[0x0A] = RegA;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0A], 86);
    EXPECT_EQ(cpu.V[0x0F], 0x1);

    cpu.reset(memory);
    RegA = 0x2B;
    memory.data[cpu.PC] = 0x8A;
    memory.data[cpu.PC+1] = 0xBE;
    cpu.V[0x0A] = RegA;
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0A], 86);
    EXPECT_EQ(cpu.V[0x0F], 0x0);
}

TEST_F(Chip8Test, SNE_VxVy) {
    Byte RegA = 0xAB;
    Byte RegB = 0xAB;
    Word orig_pc = cpu.PC;
    memory.data[cpu.PC] = 0x9A;
    memory.data[cpu.PC+1] = 0xB0;
    cpu.V[0x0A] = RegA;
    cpu.V[0x0B] = RegB;
    cpu.execute(memory);
    EXPECT_EQ(cpu.PC, orig_pc + 2);

    cpu.reset(memory);
    RegA = 0xAB;
    RegB = 0xAF;
    orig_pc = cpu.PC;
    memory.data[cpu.PC] = 0x9A;
    memory.data[cpu.PC+1] = 0xB0;
    cpu.V[0x0A] = RegA;
    cpu.V[0x0B] = RegB;
    cpu.execute(memory);
    EXPECT_EQ(cpu.PC, orig_pc+4);
}

TEST_F(Chip8Test, LD_I) {
    memory.data[cpu.PC] = 0xAA;
    memory.data[cpu.PC+1] = 0xBC;
    cpu.execute(memory);

    EXPECT_EQ(cpu.I, 0xABC);
}

TEST_F(Chip8Test, JP_V0) {
    memory.data[cpu.PC] = 0xBA;
    memory.data[cpu.PC+1] = 0xBC;
    cpu.V[0x0] = 0x09;
    cpu.execute(memory);

    EXPECT_EQ(cpu.PC, 0x09 + 0xABC);
}

TEST_F(Chip8Test, TestRandType) {
    Byte rand = cpu.randByte();
    EXPECT_EQ(1, 1);
}

TEST_F(Chip8Test, SuboutineTest) {
    memory.data[cpu.PC] = 0x2A;
    memory.data[cpu.PC+1] = 0x00;
    memory.data[0x0A00] = 0x7A;
    memory.data[0x0A01] = 0x69;
    memory.data[0x0A02] = 0x00;
    memory.data[0x0A03] = 0xEE;
    cpu.execute(memory);
    EXPECT_EQ(cpu.PC, 0x0A00);
    EXPECT_EQ(cpu.Stack[cpu.SP-1], 0x0200 + 2);
    cpu.execute(memory);
    EXPECT_EQ(cpu.V[0x0A], 0x69);
    EXPECT_EQ(cpu.PC, 0x0A00 + 2);
    cpu.execute(memory);
    EXPECT_EQ(cpu.PC, 0x200 + 2);
    EXPECT_EQ(cpu.SP, 0x0);
}

TEST_F(Chip8Test, SpriteldTest) {
    cpu.reset(memory);
    cpu.loadSprites(memory);
    EXPECT_EQ(memory[32], 0x90);
    EXPECT_EQ(memory[33], 0x90);
    EXPECT_EQ(memory[34], 0xF0);
    cpu.display[0][16] = 1;
    EXPECT_EQ(cpu.display[0][15], 0);
    EXPECT_EQ(cpu.display[0][16], 1);

}

//TEST_F(Chip8Test, TestDRW) {
//    cpu.reset(memory);
//    cpu.loadSprites(memory);
//    // Sprite Adress
//    memory[cpu.PC] = 0xA0;
//    memory[cpu.PC+1] = 0x0F;
//    // sprite x coord
//    memory[cpu.PC+2] = 0x6A;
//    memory[cpu.PC+3] = 0x07;
//    // sprite y coord
//    memory[cpu.PC+4] = 0x6B;
//    memory[cpu.PC+5] = 0x020;
//    // draw sprite D at chord Vx, Vy N bytes long
//    memory[cpu.PC+6] = 0xDA;
//    memory[cpu.PC+7] = 0xB8;
//    // clear screen
//    memory[cpu.PC+8] = 0x00;
//    memory[cpu.PC+9] = 0xE0;
//
//    // Load sprite adress into register I
//    cpu.execute(memory);
//    EXPECT_EQ(cpu.I, 0x000F);
//    // Load coord x into VA
//    cpu.execute(memory);
//    EXPECT_EQ(cpu.V[0xA], 0x07);
//    // Load coord y into VB
//    cpu.execute(memory);
//    EXPECT_EQ(cpu.V[0xB], 0x20);
//    cpu.execute(memory);
//
//    cpu.execute(memory);
//    for (int y = 0; y < cpu.DISPN_Y; y++)
//    {
//        std::cout << '\n';
//        for (int x = 0; x < cpu.DISPN_X; x++)
//        {
//            std::cout << cpu.display[y][x] << ' '; 
//        }
//    }
//}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
