#include "game.h"

void game::game::update(){
	auto delta = GetFrameTime();
	// update player and world 
	player_.update(delta);
	world_.update(delta);

	// then process events
	// make a test event, and queue it
	std::unique_ptr<events::event> test_event = std::make_unique<events::test_event>();
	events::global_dispatcher_.queue_event(test_event);
	events::global_dispatcher_.process_events(delta);

}

void game::game::render(){
	// render the player 
	player_.render();
	// then the world
	world_.render(player_.get_frustrum());

}

environment::world& game::game::get_world(){
	return world_;
}

player::player& game::game::get_player() {
	return player_;
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
