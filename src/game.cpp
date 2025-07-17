#include "game.h"

void Game::update(){
	auto delta = GetFrameTime();
	player_.update(delta);
	// pass in player pos
	world_.update();
}

void Game::render(){
	// render the player 
	player_.render();
	// render the world based on the player chunk 
	auto camera = player_.get_camera();


	// using the camera height and width caclulate the direction of the camera 
	// and the centre point

	auto camera_direction = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
	auto camera_centre = Vector3Add(camera.position, Vector3Scale(camera_direction, RENDER_DISTANCE));
	// then make the bounding box
	BoundingBox camera_box = BoundingBox{};
	camera_box.min = Vector3{
		camera_centre.x - camera_width_ / 2,
		camera_centre.y - camera_height_ / 2, 
		camera_centre.z - RENDER_DISTANCE / 2
	};
	camera_box.min = Vector3Scale(camera_box.min, 1.2f);
	camera_box.max = Vector3{
		camera_centre.x + camera_width_ / 2,
		camera_centre.y + camera_height_ / 2,
		camera_centre.z + RENDER_DISTANCE / 2
	};
	camera_box.max = Vector3Scale(camera_box.max, 1.2f);
	world_.render(camera_box);
}

environment::world& Game::get_world(){
	return world_;
}

Player& Game::get_player() {
	return player_;
}