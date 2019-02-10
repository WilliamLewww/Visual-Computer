#pragma once
#include "core/vector2.h"
#include "core/drawing.h"

class Datas {
private:
	Vector2 position;
	int width, height;

	unsigned char data;

	inline Vector2 center() { return Vector2(position.x + (width / 2), position.y + (height / 2)); }
public:
	Datas(char data);
	void update(float elapsedTimeSeconds);
	void draw();
};