#pragma once
#include <string>
#include <sstream>
#include "core/vector2.h"
#include "core/drawing.h"

enum INSTRUCTIONS {
	INSTRUCTIONS_ADD = 1,
	INSTRUCTIONS_SUB = 2
};

struct InternalMemory {
	int registerCount;
	char *registers;

	Vector2 v_position;
	int v_width = 75, v_height = 290;
};

struct ALU {
	char *r1 = nullptr;
	char *r2 = nullptr;
	char *r3 = nullptr;
	int instruction = -1;

	Vector2 v_position;
	int v_width = 200, v_height = 150;
};

class CPU {
private:
	InternalMemory internalMemory;
	ALU aLU;

	Vector2 v_position;
	int v_width = 300, v_height = 300;

	void resetRegisters();
public:
	CPU();

	void manipulateALU();
	
	void draw();
	void drawInternalMemory();
	void drawALU();
};