#include "object.h"
#include "raymath.h"
#include "config.h"
void Ship::update(){
	Object::update(); // update the bounding box

	auto delta_time = GetFrameTime();
	// update the anchor
	anchor_.update();
	// apply gravity
	
	acceleration_.y += GRAVITY;
	acceleration_.y /= density_;


	
	// apply the sail movement coeffieicts
	DrawRectangle(800, 5, 195, 170, Fade(SKYBLUE, 0.5f));
	DrawRectangleLines(800, 5, 195, 170, BLUE);
	DrawText(TextFormat("Acceleration after gravity and buoyancy: (%06.3f, %06.3f, %06.3f)", acceleration_.x, acceleration_.y, acceleration_.z), 810, 30, 10, BLACK);

	auto sail_force = sail_.get_force();
	acceleration_ = Vector3Add(acceleration_, sail_.get_force());
	DrawText(TextFormat("Sail Force: (%06.3f, %06.3f, %06.3f)", sail_force.x, sail_force.y, sail_force.z), 810, 45, 10, BLACK);
	
	// apply the anchor force coefficient 
	auto anchor_force = anchor_.get_force();
	acceleration_ = Vector3Multiply(acceleration_, anchor_.get_force());
	DrawText(TextFormat("Anchor Force: (%06.3f, %06.3f, %06.3f)", anchor_force.x, anchor_force.y, anchor_force.z), 810, 60, 10, BLACK);
	
	// apply acceleration to velocity
	velocity_ = Vector3Scale(acceleration_, delta_time);
	DrawText(TextFormat("Velocity: (%06.3f, %06.3f, %06.3f)", velocity_.x, velocity_.y, velocity_.z), 810, 75, 10, BLACK);


	// apply the ship direction to the velocity, use sin and cos, other way around z is cos, sin is x
	auto direction_coefficient = get_direction_coefficient();
	velocity_ = Vector3Multiply(velocity_, direction_coefficient);
	DrawText(TextFormat("Direction Coefficient: (%06.3f, %06.3f, %06.3f)", direction_coefficient.x, direction_coefficient.y, direction_coefficient.z), 810, 90, 10, BLACK);
	
	// update pos
	position_ = Vector3Add(position_, velocity_);
	
	// reset accel and vel
	acceleration_ = { 0.0f, 0.0f, 0.0f };	
}

void Ship::render(){
	DrawModel(object_type_.get_model(), position_, 0.15f, WHITE);
}

std::unique_ptr<Object> Ship::clone()
{
	return std::make_unique<Ship>(*this);
}

void Ship::interact(Object* other){
	auto ocean = dynamic_cast<Ocean*>(other);
	// cast to ocean
	if (ocean != nullptr) {
		
		// buoyancy
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
	direction_ = std::fmod((direction_ + SHIP_TURN_SPEED), ( 2 * PI));
	
	// rotate the model, yaw
	// TODO: update the sail direction too with the same offest
	sail_.move_sail_left(SHIP_TURN_SPEED);

	Vector3 rotate = { 0.0f, direction_, 0.0f };
	object_type_.get_model().transform = MatrixRotateXYZ(rotate);
}

void Ship::steer_right(){
	// change ship direction to the right

	direction_ = std::fmod((direction_ - SHIP_TURN_SPEED), ( 2 * PI));
	if (direction_ < 0.0) { direction_ += 2 * PI; }

	// TODO: update the sail direction too with the same offest, here direction is decreasing
	sail_.move_sail_right(SHIP_TURN_SPEED);
	
	Vector3 rotate = {  0.0f,  direction_,  0.0f };
	object_type_.get_model().transform = MatrixRotateXYZ(rotate);
}

void Ship::raise_sail(){
	sail_.raise_sail(LOWER_RAISE_SPEED);
}

void Ship::lower_sail(){
	sail_.lower_sail(LOWER_RAISE_SPEED);
}

void Ship::turn_sail_left(){
	sail_.sail_left(direction_);
}

void Ship::turn_sail_right(){
	sail_.sail_right(direction_);
}

void Ship::set_wind(Wind* wind){
	sail_.set_wind(wind);
}
