#include "datas.h"

int color_a[3] = { 175, 175, 175 };
int color_b[3] = { 100, 100, 100 };

Datas::Datas(Vector2 position, unsigned char data) {
	this->position = position;
	this->data = data;

	width = 50 + ((int)data / 2);
	height = 50 + ((int)data / 2);
}

void Datas::update(float elapsedTimeSeconds) {
	if (position.y + height < configuration.getScreenHeight()) { 
		velocityY += 9.8 + ((float)data / 35.0);
	}
	else { 
		velocityY = 0; 
		position.y = configuration.getScreenHeight() - height;
	}

	position.x += velocityX * elapsedTimeSeconds;
	position.y += velocityY * elapsedTimeSeconds;
}

void Datas::draw() {
	drawing.drawRect(position, width, height, color_a);
	drawing.drawRectOutline(position, width, height, color_b);
	drawing.drawText(drawing.convertCharToHex(&data), center(), 1);
}