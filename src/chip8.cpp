#include "chip8.h"
#include <fstream>
#include <chrono> 
#include <random> 

chip8::chip8():randGen(std::chrono::system_clock::now().time_since_epoch().count()){
    // Most chip8 programs start at this address in memory.
    pc = ROM_START;

    memset(memory,0,sizeof(memory));

    // Load up the fonts. 
    for(unsigned int i = 0; i < FONTSET_SIZE; i++) {
        memory[FONT_START+i] = fontset[i];
    }

    //OP_00E0(); // The video buffer might be filled with random data, so lets clear the display by writing it with 0 values.

    // Initialize the random number generator. 
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U); 

    // Set up the table of function pointers.
    table[0x0] = &chip8::Table0;
    table[0x1] = &chip8::OP_1NNN;
    table[0x2] = &chip8::OP_2NNN;
    table[0x3] = &chip8::OP_3XKK;
    table[0x4] = &chip8::OP_4XKK;
    table[0x5] = &chip8::OP_5XY0;
    table[0x6] = &chip8::OP_6XKK;
    table[0x7] = &chip8::OP_7XKK;
    table[0x8] = &chip8::Table8;
    table[0x9] = &chip8::OP_9XY0;
    table[0xA] = &chip8::OP_ANNN;
    table[0xB] = &chip8::OP_BNNN; 
    table[0xC] = &chip8::OP_CXKK;
    table[0xD] = &chip8::OP_DXYN;
    table[0xE] = &chip8::TableE;
    table[0xF] = &chip8::TableF;

    table0[0x0] = &chip8::OP_00E0;
    table0[0xE] = &chip8::OP_00EE;

    table8[0x0] = &chip8::OP_8XY0;
    table8[0x1] = &chip8::OP_8XY1;
    table8[0x2] = &chip8::OP_8XY2;
    table8[0x3] = &chip8::OP_8XY3;
    table8[0x4] = &chip8::OP_8XY4;
    table8[0x5] = &chip8::OP_8XY5;
    table8[0x6] = &chip8::OP_8XY6;
    table8[0x7] = &chip8::OP_8XY7;
    table8[0xE] = &chip8::OP_8XYE;

    tableE[0x1] = &chip8::OP_EXA1;
    tableE[0xE] = &chip8::OP_EX9E;

    tableF[0x07] = &chip8::OP_FX07;
    tableF[0x0A] = &chip8::OP_FX0A;
    tableF[0x15] = &chip8::OP_FX15;
    tableF[0x18] = &chip8::OP_FX18;
    tableF[0x1E] = &chip8::OP_FX1E;
    tableF[0x29] = &chip8::OP_FX29;
    tableF[0x33] = &chip8::OP_FX33;
    tableF[0x55] = &chip8::OP_FX55;
    tableF[0x65] = &chip8::OP_FX65;
}

void chip8::loadROM(std::string filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(file.is_open()) {
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        file.seekg(0,std::ios::beg);
        file.read(buffer,size);
        file.close();

        for(long i = 0; i < size; i++) {
            memory[ROM_START+i] = buffer[i];
        }
        delete[] buffer;
    }
}

void chip8::cycle() {
    opCode = (memory[pc] << 8u) | memory[pc+1]; // Fetch.
    pc += 2; // Increment. 
    ((*this).*(table[(opCode & 0xF000u) >> 12u]))(); // Decode and Execute. 
    if(delayTimer > 0) --delayTimer; // Decrement the delay timer if needed. 
    if(soundTimer > 0) --soundTimer; // Decrement the sound timer if needed. 
}

void chip8::Table0() {
    ((*this).*(table0[opCode & 0x000Fu]))();
}

void chip8::Table8() {
    ((*this).*(table8[opCode & 0x000Fu]))();
}

void chip8::TableE() {
    ((*this).*(tableE[opCode & 0x000Fu]))();
}

void chip8::TableF() {
    ((*this).*(tableF[opCode & 0x00FFu]))();
}

void chip8::OP_NULL() {

}

void chip8::OP_00E0() {
    memset(video,0,sizeof(video));
}

void chip8::OP_00EE() {
    --sp;
    pc = stack[sp];
}

void chip8::OP_1NNN() {
    uint16_t address = opCode & 0x0FFFu;
    pc = address; 
} 

void chip8::OP_2NNN() {
    uint16_t address = opCode & 0x0FFFu;
    stack[sp] = pc; 
    ++sp;
    pc = address; 
}

void chip8::OP_3XKK() {
    uint8_t kk = opCode & 0x00FFu;
    uint8_t x = (opCode & 0x0F00u) >> 8u;

    if(registers[x] == kk) pc += 2;
}

void chip8::OP_4XKK() {
    uint8_t kk = opCode & 0x00FFu;
    uint8_t x = (opCode & 0x0F00u) >> 8u;

    if(registers[x] != kk) pc += 2;
} 

void chip8::OP_5XY0() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t y = (opCode & 0x00F0u) >> 4u;

    if(registers[x] == registers[y]) pc +=2;
} 

void chip8::OP_6XKK() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t kk = (opCode & 0x00FFu);

    registers[x] = kk; 
} 

void chip8::OP_7XKK() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t kk = (opCode & 0x00FFu);
    
    registers[x] += kk;
} 

void chip8::OP_8XY0() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t y = (opCode & 0x00F0u) >> 4u;

    registers[x] = registers[y];
} 

void chip8::OP_8XY1() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t y = (opCode & 0x00F0u) >> 4u;

    registers[x] |= registers[y];
} 

void chip8::OP_8XY2() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t y = (opCode & 0x00F0u) >> 4u;

    registers[x] &= registers[y];
}

void chip8::OP_8XY3() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t y = (opCode & 0x00F0u) >> 4u;

    registers[x] ^= registers[y];
} 

void chip8::OP_8XY4() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t y = (opCode & 0x00F0u) >> 4u;

    uint16_t res = registers[x] + registers[y];

    registers[0xF] = (res > 255u) ? 1 : 0; 

    registers[x] = res & 0xFFu; 
} 

void chip8::OP_8XY5() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t y = (opCode & 0x00F0u) >> 4u;

    registers[0xF] = (registers[x] > registers[y]) ? 1 : 0; 

    registers[x] -= registers[y];
}

void chip8::OP_8XY6() {
    uint8_t x = (opCode & 0x0F00u) >> 8u; 

    registers[0xF] = registers[x] & 0x1u; 

    registers[x] >>= 1; 
} 

void chip8::OP_8XY7() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t y = (opCode & 0x00F0u) >> 4u;

    registers[0xF] = (registers[y] > registers[x]) ? 1 : 0;

    registers[x] = registers[y] - registers[x]; 
} 

void chip8::OP_8XYE() {
    uint8_t x = (opCode & 0x0F00u) >> 8u; 

    registers[0xF] = (registers[x] & 0x80u) >> 7u; 

    registers[x] <<= 1; 
}

void chip8::OP_9XY0() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t y = (opCode & 0x00F0u) >> 4u;

    if(registers[x] != registers[y]) pc += 2; 
} 

void chip8::OP_ANNN() {
    uint16_t nnn = opCode & 0x0FFFu; 
    index = nnn; 
} 

void chip8::OP_BNNN() {
    uint16_t nnn = opCode & 0x0FFFu; 
    pc = registers[0] + nnn; 
}

void chip8::OP_CXKK() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t kk = (opCode & 0x00FFu); 

    registers[x] = randByte(randGen) & kk;
}

void chip8::OP_DXYN() {

    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t y = (opCode & 0x00F0u) >> 4u;
    uint8_t height = opCode & 0x000Fu; 

    // Wrap if needed. 
    uint8_t xPos = registers[x] % VIDEO_WIDTH;
    uint8_t yPos = registers[y] % VIDEO_HEIGHT;

    // Assume no collision initially. 
    registers[0xF] = 0; 

    for(unsigned int r = 0; r < height; ++r) {
        uint8_t spriteByte = memory[index + r];
        for(unsigned int c = 0; c < 8; ++c) {
            uint8_t spritePixel = spriteByte & (0x80u >> c); // Get the pixel from memory. 
            uint32_t* screenPixel = &video[(yPos+r)*VIDEO_WIDTH+(xPos+c)]; // Find the corresponsing pixel on the actual display. 
            if(spritePixel) { // If the pixel is a 1, lets do some checks. 
                if(*screenPixel == 0xFFFFFFFF) {
                    registers[0xF] = 1; // Collision detected. 
                }
                *screenPixel ^= 0xFFFFFFFF; // Write over the current pixel on the display. 
            }
        }
    }
}

void chip8::OP_EX9E() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t key = registers[x]; 

    if(keypad[key]) {
        pc += 2; 
    }
}

void chip8::OP_EXA1() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t key = registers[x];

    if(!keypad[key]) {
        pc += 2;
    }
}

void chip8::OP_FX07() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    registers[x] = delayTimer; 
}

void chip8::OP_FX0A() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;

    if(keypad[0]) {
        registers[x] = 0;
    } else if(keypad[1]) {
        registers[x] = 1; 
    } else if(keypad[2]) {
        registers[x] = 2; 
    } else if(keypad[3]) {
        registers[x] = 3; 
    } else if(keypad[4]) {
        registers[x] = 4; 
    } else if(keypad[5]) {
        registers[x] = 5; 
    } else if(keypad[6]) {
        registers[x] = 6; 
    } else if(keypad[7]) {
        registers[x] = 7; 
    } else if(keypad[8]) {
        registers[x] = 8; 
    } else if(keypad[9]) {
        registers[x] = 9; 
    } else if(keypad[10]) {
        registers[x] = 10; 
    } else if(keypad[11]) {
        registers[x] = 11; 
    } else if(keypad[12]) {
        registers[x] = 12; 
    } else if(keypad[13]) {
        registers[x] = 13; 
    } else if(keypad[14]) {
        registers[x] = 14; 
    } else if(keypad[15]) {
        registers[x] = 15; 
    } else {
        pc -= 2; 
    }
} 

void chip8::OP_FX15() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;

    delayTimer = registers[x];
} 

void chip8::OP_FX18() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;

    soundTimer = registers[x];
}

void chip8::OP_FX1E() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    index += registers[x];
}

void chip8::OP_FX29() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    index = FONT_START + (registers[x]*5);
}

void chip8::OP_FX33() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    uint8_t val = registers[x]; 

    memory[index + 2] = val%10;
    val = val >> 1; 
    memory[index + 1] = val%10;
    val = val >> 1;
    memory[index] = val%10;
} 

void chip8::OP_FX55() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;
    
    for(uint8_t i = 0; i <= x; ++i) {
        memory[index+i] = registers[i];
    }
} 

void chip8::OP_FX65() {
    uint8_t x = (opCode & 0x0F00u) >> 8u;

    for(uint8_t i = 0; i <= x; ++i) {
        registers[i] = memory[index+i];
    }
}