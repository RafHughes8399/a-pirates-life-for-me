#include "object.h"
#include "raymath.h"
#include "../game/config.h"
void Ship::update(){

	Object::update(); // update the bounding box

	// update the ship position
	auto delta_time = GetFrameTime();

	// so the ship currently should just fall through world
	// temporarily move the ship forward

	// update the anchor
	anchor_.update();
	

	// eventually TODO, get the anchor force coefficient

	acceleration_.y += GRAVITY;
	acceleration_.y /= density_;
	
	// factor in buoyancy and wind, etc. 

	// apply the ship directoin to the velocity, use sin and cos 
	velocity_ = Vector3Scale(acceleration_, delta_time);
	position_ = Vector3Add(position_, velocity_);
	acceleration_ = { 0.0f, 0.0f, 0.0f };
}

void Ship::render(){
	DrawModel(model_, position_, 1.0f, DARKGREEN);
}

void Ship::interact(Object* other){
	auto ocean = dynamic_cast<Ocean*>(other);
	// cast to ocean
	if (ocean != nullptr) {
		//std::cout << "interact with ocean " << std::endl;
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

Sail Ship::get_sail(){
	return sail_;
}

Anchor Ship::get_anchor(){
	return anchor_;
}

void Ship::move_anchor(){
	// either drop or raise depending on the state
	// change the anchor stuff to move 
	anchor_.move();
}

void Ship::steer_left(){
	// change ship direction to the left
	// update direction, similar to the sail except the bounds are different, then rotate the model
	// use modulus in terms of 2 * pi
	direction_ = std::fmod((direction_ + SHIP_TURN_SPEED), ( 2 * std::numbers::pi_v<float>));
	
	// rotate the model, yaw
	//model_.transform();
}

void Ship::steer_right(){
	// change ship direction to the right
	direction_ = std::fmod((direction_ - SHIP_TURN_SPEED), ( 2 * std::numbers::pi_v<float>));
	if (direction_ < 0) {
		direction_ += 2 * std::numbers::pi_v<float>;
	}
}

void Ship::raise_sail(){
	sail_.raise_sail(LOWER_RAISE_SPEED);
}

void Ship::lower_sail(){
	sail_.lower_sail(LOWER_RAISE_SPEED);
}

void Ship::turn_sail_left(){
	sail_.sail_left();
}

void Ship::turn_sail_right(){
	sail_.sail_right();
}
