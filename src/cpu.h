#pragma once
#include <string>
#include <sstream>
#include "core/vector2.h"
#include "core/drawing.h"
#include "core/input.h"

enum INSTRUCTIONS {
	INSTRUCTIONS_MOV = 0,
	INSTRUCTIONS_ADD = 1,
	INSTRUCTIONS_SUB = 2
};

struct RegisterContainer {
	Vector2 v_position;
	int v_width = 75, v_height = 20;

	unsigned char *data = nullptr;
	std::string label = " ";

	int region = -1;
};

struct InternalMemory {
	int registerCount;
	unsigned char *registers;

	Vector2 v_position;
	int v_width = 75, v_height = 290;

	RegisterContainer* registerContainer;
};

struct ALU {
	unsigned char *r1 = nullptr;
	unsigned char *r2 = nullptr;
	unsigned char *r3 = nullptr;
	std::string numberInput = "";

	int instruction = -1;

	bool activation = false;

	Vector2 v_position;
	int v_width = 200, v_height = 150;
	std::string label = " ";

	RegisterContainer* registerContainer;
};

class CPU {
private:
	InternalMemory internalMemory;
	ALU aLU;

	unsigned char* selectedRegister = nullptr;

	Vector2 v_position;
	int v_width = 300, v_height = 300;

	float velocityX, velocityY;

	inline Vector2 getInternalMemoryPosition() { return v_position + internalMemory.v_position; };
	inline Vector2 getALUPosition() { return v_position + aLU.v_position; }
	inline Vector2 getRegisterContainerPosition(RegisterContainer* container) {
		if (container->region == 0) { return v_position + internalMemory.v_position + container->v_position; }
		if (container->region == 1) { return v_position + aLU.v_position + container->v_position; }

		return Vector2();
	};

	int findIndexOfRegister(unsigned char* data);
	void linkRegisterContainer(RegisterContainer* registerContainer, unsigned char* data);

	void resetRegisters();
	void updateLabelALU();

	void handleInternalMemory();
	unsigned char* getClickInternalMemory();

	void handleALU();

	void movALU();
	void addALU();
	void subALU();

	void resetALU();
public:
	CPU();

	void manipulateALU();

	void update(float elapsedTimeSeconds);
	
	void draw();
	void drawInternalMemory();
	void drawALU();
	void drawRegisterContainer(RegisterContainer* registerContainer);
};