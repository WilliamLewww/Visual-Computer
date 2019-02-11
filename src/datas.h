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
public:
	inline Vector2 center() { return Vector2(position.x + (width / 2), position.y + (height / 2)); }
	inline double top() { return position.y; }
	inline double bottom() { return position.y + height; }
	inline double left() { return position.x; }
	inline double right() { return position.x + width; }

	inline unsigned char getData() { return data; };

	Datas(Vector2 position, unsigned char data);
	void update(float elapsedTimeSeconds);
	void draw();
};