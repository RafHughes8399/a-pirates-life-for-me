#include "ship_components.h"
#include "utility_functions.h"
#include <iostream>
float Sail::get_sail_direction(){
	return direction_;
}

float Sail::get_sail_length(){
	return length_;
}

float Sail::get_width(){
	return width_;
}

Vector3 Sail::get_force(){
	return force_;
}


void Sail::sail_left(float& ship_direction) {
	auto left_bound = ship_direction + (PI / 2);
	auto right_bound = ship_direction - (PI / 2);
	
	auto new_dir = direction_ + SAIL_TURN_SPEED;
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

void Sail::sail_right(float& ship_direction) {
	auto left_bound = ship_direction + (PI / 2);
	auto right_bound = ship_direction - (PI / 2);

	auto new_dir = direction_ - SAIL_TURN_SPEED;
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
void Sail::move_sail_left(float rad){
	direction_ = std::fmod(direction_ + rad, 2 *PI);
	calculate_force();
}
void Sail::move_sail_right(float rad) {
	direction_ = std::fmod(direction_ - rad, 2 *PI);
	calculate_force();
}

void Sail::raise_sail(float length) {
	// 0 <=
	length_ = std::max(0.0f, (length_ - length));
	calculate_force();
}

void Sail::lower_sail(float length){
	// <= 1
	length_ = std::min(1.0f, (length_ + length));
	calculate_force();

}

void Sail::set_wind(float direction, float speed){
	wind_.x = direction;
	wind_.y = speed;
}

Vector2& Sail::get_wind(){
	// TODO: insert return statement here
	return wind_;
}


/**  this needs some attention */
void Sail::calculate_force(){

	// get the upper and lower bounds of the sail
	auto left = std::fmod(direction_ + length_ / 2 , PI2);
	if (left < 0.0) { left += PI2; }
	auto right = std::fmod(direction_ - length_ / 2, PI2);
	if (right  < 0.0) { right += PI2; }

	// default force, sail doesn't catch the wind
	force_ = Vector3{ (NO_WIND), 0.0, (NO_WIND) };

	if (right < left) {
		if (right <= wind_.x and wind_.x<= left) {
			float distance = std::fmod(std::abs(wind_.x - direction_), PI2);

			auto proportion = 1.0f - (distance / (length_));
			force_ = Vector3{ wind_.y * proportion, 0.0f, wind_.y * proportion };

		}
	}
	else if (left < right) {
		if (not (left <=  wind_.x and wind_.x <= right)) {
			float distance = std::fmod(std::abs(wind_.x - direction_), PI2);
			auto proportion = 1.0f - (distance / length_);
			
			force_ = Vector3{ wind_.y * proportion, 0.0f, wind_.x * proportion };
		
		}
	}

	// scale the force by th length of the sail
	force_ = Vector3Scale(force_, length_);
	DrawText(TextFormat("Sail force: (%06.3f, %06.3f, %06.3f)", force_.x, force_.y, force_.z), 810, 120, 10, BLACK);

}

void Anchor::move(){
	state_->move(this);
	// move the anchor, calculate the force coeff.
}

void Anchor::update() {
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

Vector3 Anchor::get_force(){
	return force_coefficient_;
}

float Anchor::get_depth(){
	return depth_;
}

float Anchor::get_speed(){
	return state_->get_speed();
}

void Anchor::calculate_force(){
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

float Anchor::AnchorState::get_speed()
{
	return speed_;
}

void Anchor::StationaryState::move(Anchor* anchor){
	// start moving the anchor, depending on the depth
	if (anchor->depth_ == 0.0f) {
		anchor->state_.reset(new MovingState(ANCHOR_DROP_SPEED));
	}
	else if (anchor->depth_ == ANCHOR_MAX_DEPTH) {
		anchor->state_.reset(new MovingState(-ANCHOR_DROP_SPEED));
	}
}

void Anchor::MovingState::move(Anchor* anchor) {
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
