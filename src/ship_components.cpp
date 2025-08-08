#include "ship_components.h"
#include "utility_functions.h"
#include <iostream>
float components::sail::get_sail_direction(){
	return direction_;
}

float components::sail::get_sail_length(){
	return length_;
}

float components::sail::get_width(){
	return width_;
}

Vector3 components::sail::get_force(){
	return force_;
}

// TODO implement
void components::sail::turn(float delta, float ship_direction, int turn_direction){
	(void) delta;
	(void) ship_direction;
	(void) turn_direction;

	auto left_bound = std::fmod(ship_direction + (PI /2), PI2);
	auto right_bound = std::fmod(ship_direction - (PI / 2), PI2);

	auto turn = SAIL_TURN_SPEED * delta * turn_direction;
	auto new_direction = std::fmod(direction_ + turn, PI2);
	direction_ = new_direction;
	calculate_force();
	return;
}

//TODO implement
void components::sail::move(float length, int direction){
	auto move = length * direction;
	auto new_length = length_ + move;
	
	// ensure length is within bounds 

	// if new length would be < 0
	length_ = std::max(0.0f, new_length);
	// if new length would be > 0
	length_ = std::min(new_length, 1.0f);
}

// these are for when the sail is being moved along with the ship

void components::sail::set_wind(float direction, float speed){
	wind_.x = direction;
	wind_.y = speed;
}

Vector2& components::sail::get_wind(){
	return wind_;
}

void components::sail::calculate_force(){

	// get the upper and lower bounds of the sail
	auto left = std::fmod(direction_ + length_ / 2 , PI2);
	if (left < 0.0) { left += PI2; }
	auto right = std::fmod(direction_ - length_ / 2, PI2);
	if (right  < 0.0) { right += PI2; }

	// default force, sail doesn't catch the wind
	force_ = Vector3{ (NO_WIND), 0.0, (NO_WIND) };
	float distance = std::fmod(std::abs(wind_.x - direction_), PI2);
	float max_distance = PI2;
	float proportion = 1 - (distance / max_distance);
	force_ = Vector3Add(Vector3{ wind_.y * proportion, 0.0f, wind_.y * proportion }, force_);
	// scale the force by th length of the sail
	force_ = Vector3Scale(force_, length_);
	DrawText(TextFormat("components::sail force: (%06.3f, %06.3f, %06.3f)", force_.x, force_.y, force_.z), 810, 120, 10, BLACK);

}

void components::anchor::move(float depth, int direction){
	auto move = depth * direction;
	auto new_depth = depth_ + move;
	
	// ensure depth is within bounds 

	// if new depth would be < 0
	depth_ = std::max(0.0f, new_depth);
	// if new depth would be > 0
	depth_ = std::min(new_depth, 1.0f);
	calculate_force();
}
Vector3 components::anchor::get_force(){
	return force_coefficient_;
}

float components::anchor::get_depth(){
	return depth_;
}


void components::anchor::calculate_force(){
	auto depth_ratio = 1 * (depth_ / ANCHOR_MAX_DEPTH);
	force_coefficient_ = Vector3{1.0f - depth_ratio, 1.0f, 1.0f - depth_ratio};
}
