#include "world.h"
#include "raymath.h"
void World::update(){
	for (auto& o : world_objects_) {
		o->update();
	}
	player_.update();
}

void World::render() {
	DrawGrid(1000, 1);
	for (auto& o : world_objects_) {
		o->render();
	}
	player_.render();
}

Player World::get_player(){
	return player_;
}

Object*  World::get_ocean(){
	return world_objects_[0].get();
}
