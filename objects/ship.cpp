#include "object.h"
#include "raymath.h"
#include "../game/config.h"
void Ship::update(){
	// update the ship position
	auto delta_time = GetFrameTime();

	// so the ship currently should just fall through world
	// temporarily move the ship forward

	auto acceleration = Vector3{ 0.0,0.0,0.0 };
	acceleration.y = GRAVITY / density_;
	
	// factor in buoyancy and wind, etc. 
	velocity_ = Vector3Scale(acceleration, delta_time);
	position_ = Vector3Add(position_, velocity_);
}

void Ship::render(){
	DrawModel(model_, position_, 1.0f, DARKGREEN);
}

void Ship::interact(Object* other){
	auto ocean = dynamic_cast<Ocean*>(other);
	// cast to ocean

}

void Ship::set_position(Vector3 position) {
	position_ = position;
}

Vector3 Ship::get_position(){
	return position_;
}
