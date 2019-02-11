#include "joiner.h"

Joiner joiner;

bool sortDatas(Datas a, Datas b) { return (b.getData() < a.getData()); }

void Joiner::initialize() {
	cPU = CPU();
	datasCount = cPU.getRegisterAddr(0);
	datasInterval = cPU.getRegisterAddr(1);

	datasList.emplace_back(Vector2(-50, -50), 0);
}

void Joiner::update() {
	handleDatas();

	cPU.update(timer.getTimeSeconds());
	for (Datas& datas : datasList) { 
		datas.update(timer.getTimeSeconds());

		if (cPU.checkCollision(datas)) {
			cPU.handleCollision(datas);
		}
	}
}

void Joiner::draw() {
	glPushMatrix();
	for (Datas datas : datasList) { datas.draw(); };
	cPU.draw();
	glPopMatrix();
}

void Joiner::generateRandomDatas() {
	int randData = rand() % (256 - 1 + 1) + 1;
	datasList.emplace_back(Vector2(rand() % (configuration.getScreenWidth() - (50 + randData) + 1), -(randData + 50)), randData);
	std::sort(datasList.begin(), datasList.end(), sortDatas);
}

void Joiner::handleDatas() {
	if (*datasCount > 0) {
		if (datasTimer >= *datasInterval) {
			generateRandomDatas();
			datasTimer = 0.0;
			*datasCount -= 1;
		}
		else {
			datasTimer += timer.getTimeSeconds();
		}
	}
}