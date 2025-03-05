#include "ocean.h"
#define OCEAN_ORIGIN {0.0f,0.0f,0.0f}
void Ocean::update(){
	return;
}

void Ocean::render() {
	DrawGrid(1000, 1);
	DrawModel(ocean_model_, Vector3{ 0.0f, (depth_ / 2) * -1, 0.0f }, 1.0f, BLUE);
}

float Ocean::get_density() {
	return density_;
}
