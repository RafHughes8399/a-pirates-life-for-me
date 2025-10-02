#include "game.h"

void game::game::update(){
	auto delta = GetFrameTime();
	// update player and world 
	player_.update(delta);
	world_.update(delta);

	events::global_dispatcher_.process_events(delta);

}

void game::game::render(){
	// render the world
	BeginMode3D(player_.get_camera());

		world_.render(player_.get_frustrum());

	EndMode3D();
	// then the player, so the hud appears on top
	player_.render();
	//DrawRectangle(100, 100, 100, 100, RED); // testing 2d drawing after 3d

}

environment::world& game::game::get_world(){
	return world_;
}

player::player& game::game::get_player() {
	return player_;
}


entities::entity* game::game::get_object(int id){
	return world_.get_entity(id);
}

void game::test_game::update(){
	auto delta = GetFrameTime();
	player_.update(delta);
	world_.update(delta);
}

void game::test_game::render(){
	world_.render(player_.get_frustrum());


}

environment::world& game::test_game::get_world(){
	return world_;
}

player::test_player& game::test_game::get_player() {
	return player_;
}
