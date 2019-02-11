#pragma once
#include <random>
#include <vector>
#include "core/timer.h"
#include "core/configuration.h"
#include "cpu.h"
#include "datas.h"

class Joiner {
private:
	CPU cPU;

	std::vector<Datas> datasList;
	unsigned char *datasCount = nullptr, *datasInterval = nullptr;
	float datasTimer = 0.0;

	void generateRandomDatas();
	void handleDatas();
public:
	int seed;
	
	void initialize();
	void update();
	void draw();
};

extern Joiner joiner;