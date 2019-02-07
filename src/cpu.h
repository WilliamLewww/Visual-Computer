#pragma once
#include <string>
#include <sstream>
#include "core/vector2.h"
#include "core/drawing.h"
#include "core/input.h"

enum INSTRUCTIONS {
	INSTRUCTIONS_ADD = 1,
	INSTRUCTIONS_SUB = 2
};

struct RegisterContainer {
	Vector2 v_position;
	int v_width = 75, v_height = 20;

	char *data = nullptr;
	std::string label = " ";
};

struct InternalMemory {
	int registerCount;
	char *registers;

	Vector2 v_position;
	int v_width = 75, v_height = 290;

	RegisterContainer* registerContainer;
};

struct ALU {
	char *r1 = nullptr;
	char *r2 = nullptr;
	char *r3 = nullptr;
	int instruction = -1;

	Vector2 v_position;
	int v_width = 200, v_height = 150;
	std::string label = " ";

	RegisterContainer* registerContainer;
};

class CPU {
private:
	InternalMemory internalMemory;
	ALU aLU;

	char* selectedRegister = nullptr;

	Vector2 v_position;
	int v_width = 300, v_height = 300;

	float velocityX, velocityY;

	int findIndexOfRegister(char* data);
	void linkRegisterContainer(RegisterContainer* registerContainer, char* data);

	void resetRegisters();
	void updateLabelALU();

	void handleInternalMemory();
	char* getClickInternalMemory();

	void handleALU();
public:
	CPU();

	void manipulateALU();

	void update();
	
	void draw();
	void drawInternalMemory();
	void drawALU();
	void drawRegisterContainer(RegisterContainer* registerContainer);
};