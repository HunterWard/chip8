#include <iostream>
#include "cpu.h"

// Built in sprites - 0-9/A-F
unsigned char baseSprites[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80
};

// loadRom
// Implementation is copy and pasted from a C implementation
// Needs to be optimized for C++
bool cpu::loadROM(const char* f) 
{
    FILE* romF = fopen(f, "rb");
    if (romF == NULL) 
    {
        printf("CANT OPEN");
        return false;
    }

    fseek(romF, 0L, SEEK_END);
    long fileSize = ftell(romF);
    rewind(romF);

    char* buffer = (char*)malloc(sizeof(char) * fileSize);
    if (buffer == NULL) 
    {
        printf("BUFFER");
        return false;
    }

    size_t readFile = fread(buffer, sizeof(char), fileSize, romF);
    if (readFile != fileSize) 
    {
        printf("Size Mismatch");
        return false;
    }

    if ((4096 - 512) > fileSize) 
    {
        for (int i = 0; i < fileSize; ++i) 
        {
            mem[i + 512] = buffer[i];
        }
    }

    else 
    {
        printf("FILE TOO BIG, MAX 4k");
        return false;
    }

    fclose(romF);
    free(buffer);

    return true;
}

void cpu::cpuCycle() {
    // Opcodes are 2 bytes
    unsigned short opCode = mem[pc] << 8 | mem[pc + 1];

    // First byte is key
    switch (opCode & 0xF000)
    {
    case 0x0000:
        switch (opCode & 0x00FF)
        {
        // 0x00E0 CLS - Clear display buffer
        case 0x00E0: 

            for (int i = 0; i < 2048; ++i) 
            {
                display[i] = 0x00;
            }
            update = true;
            pc = pc + 2;
            break;

         // 0x00EE RET - Return from subroutine
        case 0x00EE: 

            --sp;
            pc = stack[sp];

            break;

        default:
            std::cout << "0x0NNN case?" << std::endl;
        }
        break;

    case 0x1000:
        pc = opCode & 0x0FFF;
        break;

    // 0x2NNN - 
    case 0x2000:
        stack[sp] = pc;
        ++sp;
        pc = opCode & 0x0FFF;
        break;

    case 0x3000:
        if (R[(opCode & 0x0F00) >> 8] == (opCode & 0x00FF)) {
            pc += 4;
        }
        else {
            pc += 2;
        }
        

    default:
        break;
    }

}

// Constructor
cpu::cpu()
{
    for (int i = 0; i < 2048; i++) 
    {
        display[i] = 0;
    }

    // Stack, input and registers
    for (int i = 0; i < 16; i++) 
    {
        input[i] = R[i] = stack[i] = 0;
    }

    pc = 0x200;
    op = 0x0;
    ir = 0x0;
    sp = 0x0;

    // Memory
    for (int i = 0; i < 4096; i++) 
    {
        mem[i] = 0;
    }

    // Timers
    delay = 0;
    sound = 0;

    // Built in sprites - mem[0x000 to 0x1FF]
    for (int i = 0; i < 80; i++) 
    {
        mem[i] = baseSprites[i];
    }

    // Screen update
    update = true;
}

