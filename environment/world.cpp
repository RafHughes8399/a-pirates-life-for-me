#include "world.h"
#include "raymath.h"
void World::update(){
	ocean_.update();
	player_.update();
}

void World::render() {
	ocean_.render();
	player_.render();
}

Player World::get_player(){
	return player_;
}
