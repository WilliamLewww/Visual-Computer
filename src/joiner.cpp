#include "joiner.h"

Joiner joiner;

bool sortDatas(Datas a, Datas b) { return (b.getData() < a.getData()); }

void Joiner::initialize() {
	cPU = CPU();
	datasCount = cPU.getRegisterAddr(0);
	datasInterval = cPU.getRegisterAddr(1);

	datasList.emplace_back(Vector2(configuration.getScreenWidth() - 200, configuration.getScreenHeight() - 200), 7);
}

void Joiner::update() {
	handleDatas();

	cPU.update(timer.getTimeSeconds());

	for (int x = 0; x < datasList.size(); x++) {
		datasList[x].update(timer.getTimeSeconds());

		if (cPU.checkCollision(datasList[x])) {
			cPU.handleCollision(datasList[x]);
		}

		if (!datasList[x].getAlive()) {
			datasList.erase(datasList.begin() + x);
			x -= 1;
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
		else { datasTimer += timer.getTimeSeconds(); }
	}
}