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

CPU::CPU() {
	v_position = Vector2(50, 50);

	internalMemory.registerCount = 10;
	internalMemory.registers = new char[internalMemory.registerCount];
	resetRegisters();

	internalMemory.registerContainer = new RegisterContainer[internalMemory.registerCount];
	internalMemory.v_position = v_position + Vector2(220, 5);
	for (int x = 0; x < internalMemory.registerCount; x++) {
		linkRegisterContainer(&internalMemory.registerContainer[x], &internalMemory.registers[x]);
		internalMemory.registerContainer[x].v_position = internalMemory.v_position + Vector2(0, 20 * x);
	}

	aLU.registerContainer = new RegisterContainer[3];
	aLU.v_position = v_position + Vector2(110 - (aLU.v_width / 2), 145);
	aLU.registerContainer[0].v_position = aLU.v_position + Vector2(10, 10);
	aLU.registerContainer[1].v_position = aLU.v_position + Vector2(115, 10);
	aLU.registerContainer[2].v_position = aLU.v_position + Vector2(62.5, 45);
}

int CPU::findIndexOfRegister(char* data) {
	for (int x = 0; x < internalMemory.registerCount; x++) {
		if (data == &internalMemory.registers[x]) {
			return x;
		}
	}

	return -1;
}

void CPU::linkRegisterContainer(RegisterContainer* registerContainer, char* data) {
	registerContainer->data = data;
	registerContainer->label = "$r" + std::to_string(findIndexOfRegister(data)) + ":";
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

	v_position.x += velocityX;
	v_position.y += velocityY;
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

		if (input.getMouseX() > aLU.v_position.x + 10 && input.getMouseX() < aLU.v_position.x + 85 &&
			input.getMouseY() > aLU.v_position.y + 10 && input.getMouseY() < aLU.v_position.y + 30) {
			if (selectedRegister != nullptr) {
				aLU.r1 = selectedRegister;
				linkRegisterContainer(&aLU.registerContainer[0], aLU.r1);
				selectedRegister = nullptr;
			}
			else { aLU.r1 = nullptr; aLU.registerContainer[0].data = nullptr; }
		}

		if (input.getMouseX() > aLU.v_position.x + 115 && input.getMouseX() < aLU.v_position.x + 190 &&
			input.getMouseY() > aLU.v_position.y + 10 && input.getMouseY() < aLU.v_position.y + 30) {
			if (selectedRegister != nullptr) {
				aLU.r2 = selectedRegister;
				linkRegisterContainer(&aLU.registerContainer[1], aLU.r2);
				selectedRegister = nullptr;
			}
			else { aLU.r2 = nullptr; aLU.registerContainer[1].data = nullptr; }
		}

		if (input.getMouseX() > aLU.v_position.x + 62.5 && input.getMouseX() < aLU.v_position.x + 137.5 &&
			input.getMouseY() > aLU.v_position.y + 45 && input.getMouseY() < aLU.v_position.y + 65) {
			if (selectedRegister != nullptr) {
				aLU.r3 = selectedRegister;
				linkRegisterContainer(&aLU.registerContainer[2], aLU.r3);
				selectedRegister = nullptr;
			}	
			else { aLU.r3 = nullptr; aLU.registerContainer[2].data = nullptr; }
		}

		updateLabelALU();
	}
}

void CPU::draw() {
	drawing.drawRect(v_position, v_width, v_height, colorCPU);
	drawing.drawRectOutline(v_position, v_width, v_height);
	drawInternalMemory();
	drawALU();
}

void CPU::drawInternalMemory() {
	drawing.drawRect(internalMemory.v_position, internalMemory.v_width, internalMemory.v_height, colorRegisterNull);
	drawing.drawRectOutline(internalMemory.v_position, internalMemory.v_width, internalMemory.v_height);

	for (int x = 0; x < internalMemory.registerCount; x++) {
		drawRegisterContainer(&internalMemory.registerContainer[x]);
	}
}

void CPU::drawALU() {
	drawing.drawRect(aLU.v_position, aLU.v_width, aLU.v_height, colorALU);
	drawing.drawRectOutline(aLU.v_position, aLU.v_width, aLU.v_height);

	drawRegisterContainer(&aLU.registerContainer[0]);
	drawRegisterContainer(&aLU.registerContainer[1]);
	drawRegisterContainer(&aLU.registerContainer[2]);

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

void CPU::updateLabelALU() {
	aLU.label.clear();

	switch (aLU.instruction) {
		case 0: aLU.label += "add   "; break;
		case 1: aLU.label += "sub"; break;
	}

	bool addComma = false;

	if (aLU.r3 != nullptr) { aLU.label += "$r"; aLU.label.push_back(findIndexOfRegister(aLU.r3) + '0'); addComma = true; }
	if (addComma) { aLU.label += ","; addComma = false; }
	if (aLU.r1 != nullptr) { aLU.label += "$r"; aLU.label.push_back(findIndexOfRegister(aLU.r1) + '0'); addComma = true; }
	if (addComma) { aLU.label += ","; addComma = false; }
	if (aLU.r2 != nullptr) { aLU.label += "$r"; aLU.label.push_back(findIndexOfRegister(aLU.r2) + '0'); }

	if (aLU.label.empty()) { aLU.label = " "; }
}

void CPU::drawRegisterContainer(RegisterContainer* registerContainer) {
	if (registerContainer->data == nullptr) {  drawing.drawRect(registerContainer->v_position, registerContainer->v_width, registerContainer->v_height, colorALUNull); }
	else { 
		if (selectedRegister == registerContainer->data) {
			drawing.drawRect(registerContainer->v_position, registerContainer->v_width, registerContainer->v_height, colorRegisterSelected);  
		}
		else {
			drawing.drawRect(registerContainer->v_position, registerContainer->v_width, registerContainer->v_height, colorRegister);  
		}

		if (registerContainer->label != " ") {
			drawing.drawText(registerContainer->label.c_str(), registerContainer->v_position + Vector2((registerContainer->v_width / 4), 9), 1);
			drawing.drawText(convertCharToHex(registerContainer->data), registerContainer->v_position + Vector2((registerContainer->v_width / 2) + 15, 9), 1);
		}
	}
	drawing.drawRectOutline(registerContainer->v_position, registerContainer->v_width, registerContainer->v_height);
}