#include "datas.h"

int color_a[3] = { 175, 175, 175 };
int color_b[3] = { 100, 100, 100 };

Datas::Datas(char data) {
	this->data = data;
}

void Datas::update(float elapsedTimeSeconds) {

}

void Datas::draw() {
	drawing.drawRect(position, width, height, color_a);
	drawing.drawRectOutline(position, width, height, color_b);
	drawing.drawText(drawing.convertCharToHex(&data), center(), 1);
}