#include "game.h"

void Game::update(){
	player_.update();
	// pass in player pos
	world_.update();
}

void Game::render(){
	// render the player 
	player_.render();
	// render the world based on the player chunk 
	world_.render();
}

World& Game::get_world(){
	return world_;
}

Player& Game::get_player() {
	return player_;
}