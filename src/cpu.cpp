#include "cpu.h"

int colorCPU[3] = { 100, 100, 100 };
int colorALU[3] = { 90, 90, 90 };
int colorRegister[3] = { 150, 150, 150 };
int colorRegisterNull[3] = { 25, 25, 25 };

const char* convertCharToHex(char* data) {
	std::stringstream stream;
	if ((int)*data < 16) { stream << "0x0" << std::hex << (int)*data; }
	else { stream << "0x" << std::hex << (int)*data; }

	return stream.str().c_str();
}

CPU::CPU() {
	v_position = Vector2(100, 100);

	internalMemory.registerCount = 10;
	internalMemory.registers = new char[internalMemory.registerCount];
	resetRegisters();
}

void CPU::resetRegisters() {
	for (int x = 0; x < internalMemory.registerCount; x++) {
		internalMemory.registers[x] = 0;
	}
}

void CPU::manipulateALU() {
	if (aLU.instruction == INSTRUCTIONS_ADD) { *aLU.r3 = *aLU.r1 + *aLU.r2; }
	if (aLU.instruction == INSTRUCTIONS_SUB) { *aLU.r3 = *aLU.r1 - *aLU.r2; }
}

void CPU::draw() {
	drawing.drawRect(v_position, v_width, v_height, colorCPU);
	drawing.drawRectOutline(v_position, v_width, v_height);
	drawInternalMemory();
	drawALU();
}

void CPU::drawInternalMemory() {
	internalMemory.v_position = v_position + Vector2(220, 5);
	drawing.drawRectOutline(internalMemory.v_position, internalMemory.v_width, internalMemory.v_height);

	drawing.drawRect(internalMemory.v_position, internalMemory.v_width, internalMemory.v_height, colorRegisterNull);
	for (int x = 1; x <= internalMemory.registerCount; x++) {
		drawing.drawRect(internalMemory.v_position + Vector2(0, 20 * (x - 1)), internalMemory.v_width, 20, colorRegister);
		drawing.drawLine(internalMemory.v_position + Vector2(0, 20 * x), internalMemory.v_position + Vector2(internalMemory.v_width, 20 * x));

		std::string label = "r" + std::to_string(x - 1) + ":";
		drawing.drawText(label.c_str(), internalMemory.v_position + Vector2((internalMemory.v_width / 5), (20 * x) - 11), 1);
		drawing.drawText(convertCharToHex(&internalMemory.registers[x - 1]), internalMemory.v_position + Vector2((internalMemory.v_width / 2) + 15, (20 * x) - 11), 1);
	}
}

void CPU::drawALU() {
	aLU.v_position = v_position + Vector2(110 - (aLU.v_width / 2), 145);
	drawing.drawRect(aLU.v_position, aLU.v_width, aLU.v_height, colorALU);
	drawing.drawRectOutline(aLU.v_position, aLU.v_width, aLU.v_height);

	if (aLU.r1 == nullptr) { drawing.drawRect(aLU.v_position + Vector2(10, 10), 75, 20, colorRegisterNull); }
	else { drawing.drawRect(aLU.v_position + Vector2(10, 10), 75, 20, colorRegister); }
	drawing.drawRectOutline(aLU.v_position + Vector2(10, 10), 75, 20);

	if (aLU.r2 == nullptr) { drawing.drawRect(aLU.v_position + Vector2(115, 10), 75, 20, colorRegisterNull); }
	else { drawing.drawRect(aLU.v_position + Vector2(115, 10), 75, 20, colorRegister); }
	drawing.drawRectOutline(aLU.v_position + Vector2(115, 10), 75, 20);

	if (aLU.r3 == nullptr) { drawing.drawRect(aLU.v_position + Vector2(62.5, 45), 75, 20, colorRegisterNull); }
	else { drawing.drawRect(aLU.v_position + Vector2(62.5, 45), 75, 20, colorRegister); }
	drawing.drawRectOutline(aLU.v_position + Vector2(62.5, 45), 75, 20);

	if (aLU.instruction == -1) { drawing.drawRect(aLU.v_position + Vector2(50, 90), 100, 20, colorRegisterNull); }
	else { drawing.drawRect(aLU.v_position + Vector2(50, 90), 100, 20, colorRegister); }
	drawing.drawRectOutline(aLU.v_position + Vector2(50, 90), 100, 20);

	drawing.drawRect(aLU.v_position + Vector2(10, 120), 40, 20, colorRegister);
	drawing.drawRectOutline(aLU.v_position + Vector2(10, 120), 40, 20);
	drawing.drawText("add", aLU.v_position + Vector2(30, 130), 1);

	drawing.drawRect(aLU.v_position + Vector2(60, 120), 40, 20, colorRegister);
	drawing.drawRectOutline(aLU.v_position + Vector2(60, 120), 40, 20);
	drawing.drawText("sub", aLU.v_position + Vector2(80, 130), 1);
}