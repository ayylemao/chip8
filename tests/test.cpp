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
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
