#include "chip8.h"
#include "display.h"
#include <chrono>
#include <iostream>

int main(int argc, char** argv) {
    if(argc != 4) {
        std::cerr << "MISSING ARGUMENTS!" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int vidScale = std::stoi(argv[1]);
    int cycleDelay = std::stoi(argv[2]);
    std::string romFilename = argv[3]; 

    display d("Chip 8", VIDEO_WIDTH*vidScale,VIDEO_HEIGHT*vidScale,VIDEO_WIDTH,VIDEO_HEIGHT); 

    chip8 c8;
    c8.loadROM(romFilename);

    int videoPitch = sizeof(c8.video[0])*VIDEO_WIDTH;

    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    bool quit = false;

    while(!quit) {
        quit = d.processInput(c8.keypad);
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaT = std::chrono::duration<float,std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if(deltaT > cycleDelay) {
            lastCycleTime = currentTime;
            c8.cycle();
            d.update(c8.video,videoPitch);
        }
    }
    return 0; 
}