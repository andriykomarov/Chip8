#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint> 

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

class display
{
private:
    /* data */
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
public:
    display(char const* title, int winWidth, int winHeight, int textureWidth, int textureHeight);
    ~display();
    void update(void const* buffer, int pitch);
    bool processInput(uint8_t* keys);
};

#endif