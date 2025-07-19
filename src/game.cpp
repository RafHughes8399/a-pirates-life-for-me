#include "game.h"

void game::game::update(){
	auto delta = GetFrameTime();
	player_.update(delta);
	// pass in player pos
	world_.update();
}

void game::game::render(){
	// render the player 
	player_.render();
	// then the world
	world_.render();

}

environment::world& game::game::get_world(){
	return world_;
}

player::player& game::game::get_player() {
	return player_;
}