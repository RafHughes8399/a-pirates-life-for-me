#include "ship.h"
#include "raymath.h"
#include "../game/config.h"
void Ship::update(){
	// update the ship position
	auto delta_time = GetFrameTime();

	// so the ship currently should just fall through world
	// temporarily move the ship forward

	acceleration_.y = GRAVITY / mass_;
	velocity_ = Vector3Scale(acceleration_, delta_time);
	position_ = Vector3Add(position_, velocity_);
}

void Ship::render(){
	DrawModel(ship_model_, position_, 1.0f, DARKGREEN);
}

void Ship::set_position(Vector3 position) {
	position_ = position;
}

Vector3 Ship::get_position(){
	return position_;
}
