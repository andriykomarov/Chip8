#pragma once
#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <string>
#include <random>

const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;

class chip8
{
private:
    /* data */
    uint8_t registers[16]{};
    uint8_t memory[4096]{};
    uint16_t index{};
    uint16_t pc{};
    uint16_t stack[16]{};
    uint8_t sp{};
    uint8_t delayTimer{};
    uint8_t soundTimer{};
    //uint8_t keypad[16]{};
    //uint32_t video[64*32]{};
    uint16_t opCode{};
    const unsigned int ROM_START = 0x200;
    const unsigned int FONT_START = 0x50;
    static const unsigned int FONTSET_SIZE = 80;
    uint8_t fontset[FONTSET_SIZE] =
    {   
	    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	    0x20, 0x60, 0x20, 0x20, 0x70, // 1
	    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    std::default_random_engine randGen;
	std::uniform_int_distribution<uint8_t> randByte;

    void OP_NULL(); // Do nothing. Used for the function pointer table method. 
    void OP_00E0(); // Clear the display.
    void OP_00EE(); // Return from a subroutine. 
    void OP_1NNN(); // Jump to address NNN.
    void OP_2NNN(); // Call subroutine at NNN. 
    void OP_3XKK(); // Skip the next instruction if Vx = kk.
    void OP_4XKK(); // Skip the next instruction if Vx != kk.
    void OP_5XY0(); // Skip the next instruction if Vx = Vy. 
    void OP_6XKK(); // Set Vx = kk. 
    void OP_7XKK(); // Set Vx = Vx + kk. 
    void OP_8XY0(); // Set Vx = Vy. 
    void OP_8XY1(); // Set Vx = Vx | Vy. 
    void OP_8XY2(); // Set Vx = Vx & Vy. 
    void OP_8XY3(); // Set Vx = Vx ^ Vy. 
    void OP_8XY4(); // Set Vx = Vx + Vy, Vf = Carry. 
    void OP_8XY5(); // Set Vx = Vx - Vy, Vf = !Borrow. 
    void OP_8XY6(); // Set Vx = Vx >> 1, Set Vf based on the LSB. 
    void OP_8XY7(); // Set Vx = Vy - Vx, Vf = !Borrow. 
    void OP_8XYE(); // Set Vx = Vx << 1, Set Vf based on the MSB. 
    void OP_9XY0(); // Skip the next instruction if Vx != Vy. 
    void OP_ANNN(); // Set I = nnn. 
    void OP_BNNN(); // Jump to V0 + nnn. 
    void OP_CXKK(); // Set Vx = randByte & kk. 
    void OP_DXYN(); // Display an n-byte sprite starting at memory location I at position (Vx,Vy), set Vf = Collision. 
    void OP_EX9E(); // Skip the next instruction if the key corresponding with the value of Vx is pressed. 
    void OP_EXA1(); // Skip the next instruction if the key correspsoning with the value of Vx is not pressed. 
    void OP_FX07(); // Set Vx to the value of the delay timer. 
    void OP_FX0A(); // Wait for a keypress and store the value of the key in Vx. 
    void OP_FX15(); // Set the delay timer to the value of Vx. 
    void OP_FX18(); // Set the sound timer to the value of Vx.
    void OP_FX1E(); // Set I = I + Vx. 
    void OP_FX29(); // Set I to the location of the sprite corresponding to the value in Vx. 
    void OP_FX33(); // Store the binary coded decimal (BCD) representation of Vx in I, I+1, and I+2. 
    void OP_FX55(); // Store registers V0 through Vx in memory starting at location I. 
    void OP_FX65(); // Load the registers from V0 to Vx with the contents from memory starting at location I.

    void Table0();
    void Table8();
    void TableE();
    void TableF();

    typedef void (chip8::*chip8func)();
    chip8func table[0xF + 1]{&chip8::OP_NULL};
    chip8func table0[0xE + 1]{&chip8::OP_NULL};
    chip8func table8[0xE + 1]{&chip8::OP_NULL};
    chip8func tableE[0xE + 1]{&chip8::OP_NULL};
    chip8func tableF[0x65 + 1]{&chip8::OP_NULL};

public:
    chip8(/* args */);
    //~chip8();
    void loadROM(std::string filename);
    void cycle();
    uint32_t video[64*32]{};
    uint8_t keypad[16]{};
};

#endif