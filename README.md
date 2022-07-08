
<h1 align="center">
  <br>
    <img src="https://i.imgur.com/I9k5dkB.gif" alt="CHIP-8 IBM Logo" width="200"></a>
  <br>
    CHIP-8 C++
  <br>
</h1>

<h4 align="center">A Chip-8 Emulator built with C++ and <a href="https://www.libsdl.org" target="_blank">SDL</a>.</h4>

<p align="center">
  <a href="#Introduction">Introduction</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#credits">Credits</a> 
</p>

<h5 align="center">
  <img src="https://i.imgur.com/uWXNGFm.gif" alt="Chip-8 Running Space Invaders">  
</h5>

## Introduction

CHIP-8 is an interpreted programming language that ran on the CHIP-8 virtual machine, developed by Joseph Weisbecker in the mid-1970s with the purpose of simplifying the game development process for various 8-bit systems at the time. 

## How To Use

To clone and run this application, you'll need [Git](https://git-scm.com) and the [Simple DirectMedia Layer (SDL)](https://www.libsdl.org) installed on your computer. From your command line:

```bash
# Clone this repository
$ git clone https://github.com/andriykomarov/Chip8.git

# Go into the repository
$ cd Chip8

# Compile and link the necessary files, in my case being on a Mac and using clang++ the process looks something like this: 
$ clang++ src/main.cpp src/chip8.cpp src/display.cpp -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -std=c++17

# Run the with the following arguments <Video Scale> <Cycle Delay> <ROM Filename>. 
$ ./a.out 10 3 SpaceInvaders.ch8
```


## Credits

This software uses the following libraries:

* [Simple DirectMedia Layer](https://www.libsdl.org)

The following resources were used in making this project. 

* [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
  - Excelent resource on the techinical aspects of the CHIP-8 system such as the behaviour of the opcodes and more. 
* [Austin Morlan's Chip-8 Guide](https://austinmorlan.com/posts/chip8_emulator/)
  - Helped me use the SDL library to render the graphics to the display as well as frame pacing. Also introduced me to using function pointer tables for projects like these. 
* [Chip-8 Wikipedia Page](https://en.wikipedia.org/wiki/CHIP-8)
  - Great higher-level overview of the chip-8. 

The following README theme is a modified version of a theme by @amitmerchant1990 from [here](https://www.readme-templates.com).
