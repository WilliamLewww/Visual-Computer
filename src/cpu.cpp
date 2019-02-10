#include "cpu.h"

int colorCPU[3] = { 100, 100, 100 };
int colorALU[3] = { 90, 90, 90 };
int colorALUNull[3] = { 75, 75, 75 };
int colorRegister[3] = { 150, 150, 150 };
int colorRegisterNull[3] = { 25, 25, 25 };
int colorRegisterSelected[3] = { 135, 181, 255 };

const char* convertCharToHex(unsigned char* data) {
	std::stringstream stream;
	if ((int)*data < 16) { stream << "0x0" << std::hex << (int)*data; }
	else { stream << "0x" << std::hex << (int)*data; }

	return stream.str().c_str();
}

CPU::CPU() {
	v_position = Vector2(50, 50);

	internalMemory.registerCount = 10;
	internalMemory.registers = new unsigned char[internalMemory.registerCount];
	resetRegisters();

	internalMemory.registerContainer = new RegisterContainer[internalMemory.registerCount];
	internalMemory.v_position = Vector2(220, 5);
	for (int x = 0; x < internalMemory.registerCount; x++) {
		linkRegisterContainer(&internalMemory.registerContainer[x], &internalMemory.registers[x]);
		internalMemory.registerContainer[x].v_position = Vector2(0, 20 * x);
		internalMemory.registerContainer[x].region = 0;
	}

	aLU.registerContainer = new RegisterContainer[3];
	aLU.v_position = Vector2(110 - (aLU.v_width / 2), 145);
	aLU.registerContainer[0].v_position = Vector2(10, 10);
	aLU.registerContainer[0].region = 1;
	aLU.registerContainer[1].v_position = Vector2(115, 10);
	aLU.registerContainer[1].region = 1;
	aLU.registerContainer[2].v_position = Vector2(62.5, 45);
	aLU.registerContainer[2].region = 1;
}

int CPU::findIndexOfRegister(unsigned char* data) {
	for (int x = 0; x < internalMemory.registerCount; x++) {
		if (data == &internalMemory.registers[x]) {
			return x;
		}
	}

	return -1;
}

void CPU::linkRegisterContainer(RegisterContainer* registerContainer, unsigned char* data) {
	registerContainer->data = data;
	registerContainer->label = "$r" + std::to_string(findIndexOfRegister(data)) + ":";
}

void CPU::resetRegisters() {
	for (int x = 0; x < internalMemory.registerCount; x++) {
		internalMemory.registers[x] = 0;
	}
}

void CPU::manipulateALU() {
	if (aLU.instruction == INSTRUCTIONS_MOV) { movALU(); }
	if (aLU.instruction == INSTRUCTIONS_ADD) { addALU(); }
	if (aLU.instruction == INSTRUCTIONS_SUB) { subALU(); }
}

void CPU::movALU() {
	int count = 0;
	unsigned char* validRegister = nullptr;
	if (aLU.r1 != nullptr) { count += 1; validRegister = aLU.r1; }
	if (aLU.r2 != nullptr) { count += 1; validRegister = aLU.r2; }
	if (aLU.r3 != nullptr) { count += 1; validRegister = aLU.r3; }

	if (count == 1 && !aLU.numberInput.empty()) {
		*validRegister = std::stoi(aLU.numberInput);
		resetALU();
	}
}

void CPU::addALU() {
	if (aLU.numberInput.empty() && aLU.r1 != nullptr && aLU.r2 != nullptr && aLU.r3 != nullptr) {
		*aLU.r3 = *aLU.r1 + *aLU.r2;
		resetALU();
	}
}

void CPU::subALU() {
	if (aLU.numberInput.empty() && aLU.r1 != nullptr && aLU.r2 != nullptr && aLU.r3 != nullptr) {
		*aLU.r3 = *aLU.r1 - *aLU.r2;
		resetALU();
	}
}

void CPU::resetALU() {
	aLU.r1 = nullptr;
	aLU.r2 = nullptr;
	aLU.r3 = nullptr;
	aLU.numberInput = "";
	aLU.instruction = -1;

	aLU.registerContainer[0].data = nullptr;
	aLU.registerContainer[1].data = nullptr;
	aLU.registerContainer[2].data = nullptr;

	updateLabelALU();
}

void CPU::update(float elapsedTimeSeconds) {
	handleInternalMemory();
	handleALU();

	velocityX = 0;
	if (input.checkKeyDown(SDLK_RIGHT) && !input.checkKeyDown(SDLK_LEFT)) { velocityX = 100; }
	if (input.checkKeyDown(SDLK_LEFT) && !input.checkKeyDown(SDLK_RIGHT)) { velocityX = -100; }

	v_position.x += velocityX * elapsedTimeSeconds;
	v_position.y += velocityY * elapsedTimeSeconds;
}

void CPU::handleInternalMemory() {
	if (input.getLeftButtonPress() && input.getMouseX() > getInternalMemoryPosition().x && input.getMouseX() < getInternalMemoryPosition().x + internalMemory.v_width
								   && input.getMouseY() > getInternalMemoryPosition().y && input.getMouseY() < getInternalMemoryPosition().y + internalMemory.v_height) {
		
		unsigned char* tempRegister = getClickInternalMemory();
		if (tempRegister != nullptr) { selectedRegister = tempRegister; }
		else { selectedRegister = nullptr; }
	}
}

unsigned char* CPU::getClickInternalMemory() {
	int tempIndex = (input.getMouseY() - getInternalMemoryPosition().y) / 20;
	return &internalMemory.registers[tempIndex];
}

void CPU::handleALU() {
	if (input.getLeftButtonPress() && input.getMouseX() > getALUPosition().x && input.getMouseX() < getALUPosition().x + aLU.v_width
								   && input.getMouseY() > getALUPosition().y && input.getMouseY() < getALUPosition().y + aLU.v_height) {

		if (input.getMouseX() > getALUPosition().x + 10 && input.getMouseX() < getALUPosition().x + 50 &&
			input.getMouseY() > getALUPosition().y + 120 && input.getMouseY() < getALUPosition().y + 140) {
			aLU.instruction = 0;
		}

		if (input.getMouseX() > getALUPosition().x + 60 && input.getMouseX() < getALUPosition().x + 100 &&
			input.getMouseY() > getALUPosition().y + 120 && input.getMouseY() < getALUPosition().y + 140) {
			aLU.instruction = 1;
		}

		if (input.getMouseX() > getALUPosition().x + 110 && input.getMouseX() < getALUPosition().x + 150 &&
			input.getMouseY() > getALUPosition().y + 120 && input.getMouseY() < getALUPosition().y + 140) {
			aLU.instruction = 2;
		}

		if (input.getMouseX() > getALUPosition().x + 170 && input.getMouseX() < getALUPosition().x + 190 &&
			input.getMouseY() > getALUPosition().y + 120 && input.getMouseY() < getALUPosition().y + 140) {
			manipulateALU();
			aLU.activation = true;
		}

		if (input.getMouseX() > getALUPosition().x + 10 && input.getMouseX() < getALUPosition().x + 85 &&
			input.getMouseY() > getALUPosition().y + 10 && input.getMouseY() < getALUPosition().y + 30) {
			if (selectedRegister != nullptr) {
				aLU.r1 = selectedRegister;
				linkRegisterContainer(&aLU.registerContainer[0], aLU.r1);
				selectedRegister = nullptr;
			}
			else { aLU.r1 = nullptr; aLU.registerContainer[0].data = nullptr; }
		}

		if (input.getMouseX() > getALUPosition().x + 115 && input.getMouseX() < getALUPosition().x + 190 &&
			input.getMouseY() > getALUPosition().y + 10 && input.getMouseY() < getALUPosition().y + 30) {
			if (selectedRegister != nullptr) {
				aLU.r2 = selectedRegister;
				linkRegisterContainer(&aLU.registerContainer[1], aLU.r2);
				selectedRegister = nullptr;
			}
			else { aLU.r2 = nullptr; aLU.registerContainer[1].data = nullptr; }
		}

		if (input.getMouseX() > getALUPosition().x + 62.5 && input.getMouseX() < getALUPosition().x + 137.5 &&
			input.getMouseY() > getALUPosition().y + 45 && input.getMouseY() < getALUPosition().y + 65) {
			if (selectedRegister != nullptr) {
				aLU.r3 = selectedRegister;
				linkRegisterContainer(&aLU.registerContainer[2], aLU.r3);
				selectedRegister = nullptr;
			}	
			else { aLU.r3 = nullptr; aLU.registerContainer[2].data = nullptr; }
		}

		updateLabelALU();
	}

	if (input.getPressKeyListSize() > 0) {
		if (input.checkKeyPress(SDLK_1)) { aLU.numberInput += '1'; updateLabelALU(); }
		if (input.checkKeyPress(SDLK_2)) { aLU.numberInput += '2'; updateLabelALU(); }
		if (input.checkKeyPress(SDLK_3)) { aLU.numberInput += '3'; updateLabelALU(); }
		if (input.checkKeyPress(SDLK_4)) { aLU.numberInput += '4'; updateLabelALU(); }
		if (input.checkKeyPress(SDLK_5)) { aLU.numberInput += '5'; updateLabelALU(); }
		if (input.checkKeyPress(SDLK_6)) { aLU.numberInput += '6'; updateLabelALU(); }
		if (input.checkKeyPress(SDLK_7)) { aLU.numberInput += '7'; updateLabelALU(); }
		if (input.checkKeyPress(SDLK_8)) { aLU.numberInput += '8'; updateLabelALU(); }
		if (input.checkKeyPress(SDLK_9)) { aLU.numberInput += '9'; updateLabelALU(); }
		if (input.checkKeyPress(SDLK_0)) { aLU.numberInput += '0'; updateLabelALU(); }

		if (input.checkKeyPress(SDLK_BACKSPACE) && !aLU.numberInput.empty()) { aLU.numberInput.pop_back(); updateLabelALU(); }
	}
}

void CPU::draw() {
	drawing.drawRect(v_position, v_width, v_height, colorCPU);
	drawing.drawRectOutline(v_position, v_width, v_height);
	drawInternalMemory();
	drawALU();
}

void CPU::drawInternalMemory() {
	drawing.drawRect(getInternalMemoryPosition(), internalMemory.v_width, internalMemory.v_height, colorRegisterNull);
	drawing.drawRectOutline(getInternalMemoryPosition(), internalMemory.v_width, internalMemory.v_height);

	for (int x = 0; x < internalMemory.registerCount; x++) {
		drawRegisterContainer(&internalMemory.registerContainer[x]);
	}
}

void CPU::drawALU() {
	drawing.drawRect(getALUPosition(), aLU.v_width, aLU.v_height, colorALU);
	drawing.drawRectOutline(getALUPosition(), aLU.v_width, aLU.v_height);

	drawRegisterContainer(&aLU.registerContainer[0]);
	drawRegisterContainer(&aLU.registerContainer[1]);
	drawRegisterContainer(&aLU.registerContainer[2]);

	drawing.drawRect(getALUPosition() + Vector2(12.5, 90), 175, 20, colorRegister);
	drawing.drawRectOutline(getALUPosition() + Vector2(12.5, 90), 175, 20);

	drawing.drawText(aLU.label.c_str(), getALUPosition() + Vector2(100, 100), 1);

	if (aLU.instruction == 0) { drawing.drawRect(getALUPosition() + Vector2(10, 120), 40, 20, colorRegisterSelected); }
	else { drawing.drawRect(getALUPosition() + Vector2(10, 120), 40, 20, colorRegister); }
	drawing.drawRectOutline(getALUPosition() + Vector2(10, 120), 40, 20);
	drawing.drawText("mov", getALUPosition() + Vector2(30, 130), 1);

	if (aLU.instruction == 1) { drawing.drawRect(getALUPosition() + Vector2(60, 120), 40, 20, colorRegisterSelected); }
	else { drawing.drawRect(getALUPosition() + Vector2(60, 120), 40, 20, colorRegister); }
	drawing.drawRectOutline(getALUPosition() + Vector2(60, 120), 40, 20);
	drawing.drawText("add", getALUPosition() + Vector2(80, 130), 1);

	if (aLU.instruction == 2) { drawing.drawRect(getALUPosition() + Vector2(110, 120), 40, 20, colorRegisterSelected);	}
	else { drawing.drawRect(getALUPosition() + Vector2(110, 120), 40, 20, colorRegister); }
	drawing.drawRectOutline(getALUPosition() + Vector2(110, 120), 40, 20);
	drawing.drawText("sub", getALUPosition() + Vector2(130, 130), 1);

	if (aLU.activation) { drawing.drawRect(getALUPosition() + Vector2(170, 120), 20, 20, colorRegisterSelected); aLU.activation = false; }
	else { drawing.drawRect(getALUPosition() + Vector2(170, 120), 20, 20, colorRegister); }
	drawing.drawRectOutline(getALUPosition() + Vector2(170, 120), 20, 20);
	drawing.drawText("=", getALUPosition() + Vector2(180, 130), 1);
}

void CPU::updateLabelALU() {
	aLU.label.clear();

	switch (aLU.instruction) {
		case 0: aLU.label += "mov   "; break;
		case 1: aLU.label += "add   "; break;
		case 2: aLU.label += "sub   "; break;
	}

	bool addComma = false;

	if (aLU.r3 != nullptr) { aLU.label += "$r"; aLU.label.push_back(findIndexOfRegister(aLU.r3) + '0'); addComma = true; }
	if (aLU.r1 != nullptr) { 
		if (addComma) { aLU.label += ","; addComma = false; }
		aLU.label += "$r"; aLU.label.push_back(findIndexOfRegister(aLU.r1) + '0'); addComma = true; 
	}
	if (aLU.r2 != nullptr) { 
		if (addComma) { aLU.label += ","; addComma = false; }
		aLU.label += "$r"; aLU.label.push_back(findIndexOfRegister(aLU.r2) + '0'); addComma = true;
	}

	if (!aLU.numberInput.empty()) {
		if (addComma) { aLU.label += ","; addComma = false; }
		aLU.label += aLU.numberInput;
	}

	if (aLU.label.empty()) { aLU.label = " "; }
}

void CPU::drawRegisterContainer(RegisterContainer* registerContainer) {
	if (registerContainer->data == nullptr) {  drawing.drawRect(getRegisterContainerPosition(registerContainer), registerContainer->v_width, registerContainer->v_height, colorALUNull); }
	else { 
		if (selectedRegister == registerContainer->data) {
			drawing.drawRect(getRegisterContainerPosition(registerContainer), registerContainer->v_width, registerContainer->v_height, colorRegisterSelected);  
		}
		else {
			drawing.drawRect(getRegisterContainerPosition(registerContainer), registerContainer->v_width, registerContainer->v_height, colorRegister);  
		}

		if (registerContainer->label != " ") {
			drawing.drawText(registerContainer->label.c_str(), getRegisterContainerPosition(registerContainer) + Vector2((registerContainer->v_width / 4), 9), 1);
			drawing.drawText(convertCharToHex(registerContainer->data), getRegisterContainerPosition(registerContainer) + Vector2((registerContainer->v_width / 2) + 15, 9), 1);
		}
	}
	drawing.drawRectOutline(getRegisterContainerPosition(registerContainer), registerContainer->v_width, registerContainer->v_height);
}