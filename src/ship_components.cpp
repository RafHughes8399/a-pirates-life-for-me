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


void components::sail::sail_left(float& ship_direction, float delta) {
	auto left_bound = ship_direction + (PI / 2);
	auto right_bound = ship_direction - (PI / 2);
	
	auto turn = SAIL_TURN_SPEED * delta;
	auto new_dir = direction_ + turn;
	if (right_bound < left_bound) {
		// business as usual, turn left
		if (not (right_bound < new_dir and new_dir < left_bound)) {
			new_dir = left_bound;
		}
	}
	else if (left_bound < right_bound) {
		if (left_bound < new_dir and new_dir < right_bound) {
			new_dir = left_bound;
		}

	}
	direction_ = new_dir;
	calculate_force();
}

void components::sail::sail_right(float& ship_direction, float delta) {
	auto left_bound = ship_direction + (PI / 2);
	auto right_bound = ship_direction - (PI / 2);

	auto turn = SAIL_TURN_SPEED * delta;
	auto new_dir = direction_ - turn;
	if (right_bound < left_bound) {
		if (not (right_bound < new_dir and new_dir < left_bound)) {
			new_dir = right_bound;
		}
	}
	else if (left_bound < right_bound) {
		if (left_bound < new_dir and new_dir < right_bound) {
			new_dir = right_bound;
		}

	}
	direction_ = new_dir;
	calculate_force();
}

// these are for when the sail is being moved along with the ship
void components::sail::move_sail_left(float rad){
	direction_ = std::fmod(direction_ + rad, 2 *PI);
	calculate_force();
}
void components::sail::move_sail_right(float rad) {
	direction_ = std::fmod(direction_ - rad, 2 *PI);
	calculate_force();
}

void components::sail::raise_sail(float length) {
	// 0 <=
	length_ = std::max(0.0f, (length_ - length));
	calculate_force();
}

void components::sail::lower_sail(float length){
	// <= 1
	length_ = std::min(1.0f, (length_ + length));
	calculate_force();

}

void components::sail::set_wind(float direction, float speed){
	wind_.x = direction;
	wind_.y = speed;
}

Vector2& components::sail::get_wind(){
	return wind_;
}


/**  this needs some attention */
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

void components::anchor::move(){
	state_->move(this);
	// move the anchor, calculate the force coeff.
}

void components::anchor::update() {
	// update the depth 
	if (get_speed() < 0) {
		depth_ = std::max(0.0f, depth_ + get_speed());
		calculate_force();
	}
	else if (get_speed() > 0) {
		depth_ = std::min(ANCHOR_MAX_DEPTH, depth_ + get_speed());
		calculate_force();
	}
}

Vector3 components::anchor::get_force(){
	return force_coefficient_;
}

float components::anchor::get_depth(){
	return depth_;
}

float components::anchor::get_speed(){
	return state_->get_speed();
}

void components::anchor::calculate_force(){
	if (depth_ == 0.0f) {
		force_coefficient_ = Vector3{ 1.0f, 1.0f, 1.0f };
	}
	else if (depth_ == ANCHOR_MAX_DEPTH) {
		force_coefficient_ = Vector3{ 0.0f, 1.0f, 0.0f };
	}
	else {
		// a function of the depth as the depth increases
		// the force moves closer to 0
		// the numbers might change
		auto depth_ratio = depth_ / ANCHOR_MAX_DEPTH;
		force_coefficient_ = Vector3{ 1.0f * depth_ratio, 1.0f, 1.0f * depth_ratio };
	}
}


// it is about changing the state, the state has the speed

float components::anchor::AnchorState::get_speed()
{
	return speed_;
}

void components::anchor::StationaryState::move(components::anchor* anchor){
	// start moving the anchor, depending on the depth
	if (anchor->depth_ == 0.0f) {
		anchor->state_.reset(new MovingState(ANCHOR_DROP_SPEED));
	}
	else if (anchor->depth_ == ANCHOR_MAX_DEPTH) {
		anchor->state_.reset(new MovingState(-ANCHOR_DROP_SPEED));
	}
}

void components::anchor::MovingState::move(components::anchor* anchor) {
	// change the direction of the anchor
	if (0.0f < anchor->depth_ and anchor->depth_ < ANCHOR_MAX_DEPTH) {
		// change direction
		speed_ *= -1;
	}
	else {
		// the anchor is stationary
		anchor->state_.reset(new StationaryState(0.0f));
	}

}
