#include "object.h"
#include "raymath.h"
#include "../game/config.h"
void Ship::update(){
	// update the ship position
	auto delta_time = GetFrameTime();

	// so the ship currently should just fall through world
	// temporarily move the ship forward

	acceleration_.y += GRAVITY / density_;
	
	// factor in buoyancy and wind, etc. 
	velocity_ = Vector3Scale(acceleration_, delta_time);
	position_ = Vector3Add(position_, velocity_);
}

void Ship::render(){
	DrawModel(model_, position_, 1.0f, DARKGREEN);
}

void Ship::interact(Object* other){
	auto ocean = dynamic_cast<Ocean*>(other);
	// cast to ocean
	if (ocean != nullptr) {
		std::cout << "interact with ocean " << std::endl;
		auto buoynacy = Vector3{ 0.0f,0.0f,0.0f };
		auto submerged_height = std::abs(0.0f - position_.y);
		auto p = ocean->get_density();
		auto g = GRAVITY;
		auto v = get_width() * get_length() * submerged_height;
		buoynacy.y = p * g * v * -1;

		adjust_acceleration(buoynacy);
	}

}

void Ship::set_position(Vector3 position) {
	position_ = position;
}

Vector3 Ship::get_position(){
	return position_;
}
