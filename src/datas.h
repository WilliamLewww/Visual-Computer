#pragma once
#include "core/vector2.h"
#include "core/drawing.h"
#include "core/configuration.h"

class Datas {
private:
	Vector2 position;
	int width, height;
	unsigned char data;

	float velocityX, velocityY;

	inline Vector2 center() { return Vector2(position.x + (width / 2), position.y + (height / 2)); }
public:
	Datas(Vector2 position, unsigned char data);
	void update(float elapsedTimeSeconds);
	void draw();
};