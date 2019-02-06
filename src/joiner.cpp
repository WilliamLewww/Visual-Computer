#include "joiner.h"

Joiner joiner;

void Joiner::initialize() {
	cPU = CPU();
}

void Joiner::update() {
	
}

void Joiner::draw() {
	glPushMatrix();
	cPU.draw();
	glPopMatrix();
}