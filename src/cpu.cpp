#include "cpu.h"

int colorCPU[3] = { 100, 100, 100 };
int colorALU[3] = { 90, 90, 90 };
int colorALUNull[3] = { 75, 75, 75 };
int colorRegister[3] = { 150, 150, 150 };
int colorRegisterNull[3] = { 25, 25, 25 };
int colorRegisterSelected[3] = { 135, 181, 255 };

const char* convertCharToHex(char* data) {
	std::stringstream stream;
	if ((int)*data < 16) { stream << "0x0" << std::hex << (int)*data; }
	else { stream << "0x" << std::hex << (int)*data; }

	return stream.str().c_str();
}

int CPU::findIndexOfRegister(char* data) {
	for (int x = 0; x < internalMemory.registerCount; x++) {
		if (data == &internalMemory.registers[x]) {
			return x;
		}
	}

	return -1;
}

CPU::CPU() {
	v_position = Vector2(50, 50);

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

void CPU::update() {
	handleInternalMemory();
	handleALU();
}

void CPU::handleInternalMemory() {
	if (input.getLeftButtonPress() && input.getMouseX() > internalMemory.v_position.x && input.getMouseX() < internalMemory.v_position.x + internalMemory.v_width
								   && input.getMouseY() > internalMemory.v_position.y && input.getMouseY() < internalMemory.v_position.y + internalMemory.v_height) {
		
		char* tempRegister = getClickInternalMemory();
		if (tempRegister != nullptr) { selectedRegister = tempRegister; }
		else { selectedRegister = nullptr; }
	}
}

char* CPU::getClickInternalMemory() {
	int tempIndex = (input.getMouseY() - internalMemory.v_position.y) / 20;
	return &internalMemory.registers[tempIndex];
}

void CPU::handleALU() {
	if (input.getLeftButtonPress() && input.getMouseX() > aLU.v_position.x && input.getMouseX() < aLU.v_position.x + aLU.v_width
								   && input.getMouseY() > aLU.v_position.y && input.getMouseY() < aLU.v_position.y + aLU.v_height) {

		if (input.getMouseX() > aLU.v_position.x + 10 && input.getMouseX() < aLU.v_position.x + 50 &&
			input.getMouseY() > aLU.v_position.y + 120 && input.getMouseY() < aLU.v_position.y + 140) {
			aLU.instruction = 0;
		}

		if (input.getMouseX() > aLU.v_position.x + 60 && input.getMouseX() < aLU.v_position.x + 100 &&
			input.getMouseY() > aLU.v_position.y + 120 && input.getMouseY() < aLU.v_position.y + 140) {
			aLU.instruction = 1;
		}

		if (selectedRegister != nullptr) {
			if (input.getMouseX() > aLU.v_position.x + 10 && input.getMouseX() < aLU.v_position.x + 85 &&
				input.getMouseY() > aLU.v_position.y + 10 && input.getMouseY() < aLU.v_position.y + 30) {
				aLU.r1 = selectedRegister;
				selectedRegister = nullptr;
			}

			if (input.getMouseX() > aLU.v_position.x + 115 && input.getMouseX() < aLU.v_position.x + 190 &&
				input.getMouseY() > aLU.v_position.y + 10 && input.getMouseY() < aLU.v_position.y + 30) {
				aLU.r2 = selectedRegister;
				selectedRegister = nullptr;
			}

			if (input.getMouseX() > aLU.v_position.x + 62.5 && input.getMouseX() < aLU.v_position.x + 137.5 &&
				input.getMouseY() > aLU.v_position.y + 45 && input.getMouseY() < aLU.v_position.y + 65) {
				aLU.r3 = selectedRegister;
				selectedRegister = nullptr;
			}
		}

		updateLabel();
	}
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
		if (selectedRegister == &internalMemory.registers[x - 1]) {
			drawing.drawRect(internalMemory.v_position + Vector2(0, 20 * (x - 1)), internalMemory.v_width, 20, colorRegisterSelected);
		}
		else {
			drawing.drawRect(internalMemory.v_position + Vector2(0, 20 * (x - 1)), internalMemory.v_width, 20, colorRegister);
		}
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

	if (aLU.r1 == nullptr) {  drawing.drawRect(aLU.v_position + Vector2(10, 10), 75, 20, colorALUNull); }
	else { 
		drawing.drawRect(aLU.v_position + Vector2(10, 10), 75, 20, colorRegister); 

		std::string label = "r" + std::to_string(findIndexOfRegister(aLU.r1)) + ":";
		drawing.drawText(label.c_str(), aLU.v_position + Vector2((internalMemory.v_width / 5) + 10, 30 - 11), 1);
		drawing.drawText(convertCharToHex(aLU.r1), aLU.v_position + Vector2((internalMemory.v_width / 2) + 15 + 10, 30 - 11), 1);
	}
	drawing.drawRectOutline(aLU.v_position + Vector2(10, 10), 75, 20);

	if (aLU.r2 == nullptr) { drawing.drawRect(aLU.v_position + Vector2(115, 10), 75, 20, colorALUNull); }
	else { 
		drawing.drawRect(aLU.v_position + Vector2(115, 10), 75, 20, colorRegister); 

		std::string label = "r" + std::to_string(findIndexOfRegister(aLU.r2)) + ":";
		drawing.drawText(label.c_str(), aLU.v_position + Vector2((internalMemory.v_width / 5) + 115, 30 - 11), 1);
		drawing.drawText(convertCharToHex(aLU.r2), aLU.v_position + Vector2((internalMemory.v_width / 2) + 15 + 115, 30 - 11), 1);
	}
	drawing.drawRectOutline(aLU.v_position + Vector2(115, 10), 75, 20);

	if (aLU.r3 == nullptr) { drawing.drawRect(aLU.v_position + Vector2(62.5, 45), 75, 20, colorALUNull); }
	else { 
		drawing.drawRect(aLU.v_position + Vector2(62.5, 45), 75, 20, colorRegister); 

		std::string label = "r" + std::to_string(findIndexOfRegister(aLU.r3)) + ":";
		drawing.drawText(label.c_str(), aLU.v_position + Vector2((internalMemory.v_width / 5) + 62.5, 65 - 11), 1);
		drawing.drawText(convertCharToHex(aLU.r3), aLU.v_position + Vector2((internalMemory.v_width / 2) + 15 + 62.5, 65 - 11), 1);
	}
	drawing.drawRectOutline(aLU.v_position + Vector2(62.5, 45), 75, 20);

	drawing.drawRect(aLU.v_position + Vector2(12.5, 90), 175, 20, colorRegister);
	drawing.drawRectOutline(aLU.v_position + Vector2(12.5, 90), 175, 20);

	drawing.drawText(aLU.label.c_str(), aLU.v_position + Vector2(100, 100), 1);

	if (aLU.instruction == 0) { drawing.drawRect(aLU.v_position + Vector2(10, 120), 40, 20, colorRegisterSelected); }
	else { drawing.drawRect(aLU.v_position + Vector2(10, 120), 40, 20, colorRegister); }
	drawing.drawRectOutline(aLU.v_position + Vector2(10, 120), 40, 20);
	drawing.drawText("add", aLU.v_position + Vector2(30, 130), 1);

	if (aLU.instruction == 1) { drawing.drawRect(aLU.v_position + Vector2(60, 120), 40, 20, colorRegisterSelected);	}
	else { drawing.drawRect(aLU.v_position + Vector2(60, 120), 40, 20, colorRegister); }
	drawing.drawRectOutline(aLU.v_position + Vector2(60, 120), 40, 20);
	drawing.drawText("sub", aLU.v_position + Vector2(80, 130), 1);
}

void CPU::updateLabel() {
	aLU.label.clear();

	switch (aLU.instruction) {
		case 0: aLU.label += "add"; break;
		case 1: aLU.label += "sub"; break;
	}

	if (aLU.r3 != nullptr) { aLU.label += " r"; aLU.label.push_back(findIndexOfRegister(aLU.r3) + '0'); aLU.label += " "; }
	if (aLU.r1 != nullptr) { aLU.label += " r"; aLU.label.push_back(findIndexOfRegister(aLU.r1) + '0'); aLU.label += " "; }
	if (aLU.r2 != nullptr) { aLU.label += " r"; aLU.label.push_back(findIndexOfRegister(aLU.r2) + '0'); aLU.label += " "; }

	if (aLU.label.empty()) { aLU.label = " "; }
}