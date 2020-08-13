#pragma once

class cpu
{
	// Registers
	// GP Registers
	unsigned char R[16];
	// Stack
	unsigned short stack[16];
	// Special Registers
	unsigned short pc;
	unsigned short sp;
	unsigned short ir;
	unsigned short op;

	// Timers
	unsigned char delay;
	unsigned char sound;

	// main memory
	unsigned char mem[4096];

public:
	// input
	unsigned char input[16];

	// display buffer
	unsigned char display[32 * 64];

	bool update;

	cpu();
	bool loadROM(const char* f);
	void cpuCycle();

};