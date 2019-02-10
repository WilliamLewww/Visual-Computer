#pragma once
#include <vector>
#include "core/timer.h"
#include "cpu.h"
#include "datas.h"

class Joiner {
private:
	CPU cPU;

	std::vector<Datas> datasList;
public:
	int seed;
	
	void initialize();
	void update();
	void draw();
};

extern Joiner joiner;