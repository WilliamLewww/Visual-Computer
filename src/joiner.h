#pragma once
#include "core/timer.h"
#include "cpu.h"

class Joiner {
private:
	CPU cPU;
public:
	int seed;
	
	void initialize();
	void update();
	void draw();
};

extern Joiner joiner;