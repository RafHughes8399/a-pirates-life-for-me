#include "../objects/ship_components.h"
#include "../game/utility_functions.h"
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

const Wind* Sail::get_wind(){
	return wind_;
}

float Sail::sail_arc(){
	return direction_ * width_;
}



/**
 * these still don't work properly
 * 
 * chin scratch
 * ok my thoughts are, let the sail move between 0 and pi, then add to 
 */
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
}

// these are for when the sail is being moved along with the ship
void Sail::move_sail_left(float rad){
	direction_ = std::fmod(direction_ + rad, 2 *PI);
}
void Sail::move_sail_right(float rad) {
	direction_ = std::fmod(direction_ - rad, 2 *PI);
}

void Sail::raise_sail(float length) {
	// 0 <=
	length_ = std::max(0.0f, (length_ - length));
}

void Sail::lower_sail(float length){
	// <= 1
	length_ = std::min(1.0f, (length_ + length));

}

void Sail::set_wind(const Wind* wind){
	wind_ = wind;
}

void Sail::update(){
	calculate_force();
}

void Sail::calculate_force(){

	// sail direction

	// get the upper and lower bounds of the sail
	auto upper = std::fmod(direction_ + length_ / 2 , PI2);
	auto lower = std::fmod(direction_ - length_ / 2, PI2);

	// normalise to [0, 2pi]
	if (upper < 0) { upper += PI * 2; }
	if (lower < 0) { lower += PI * 2; }

	// compare to wind, how close is it to the centre, need to handle the wrap around 0 2pi case 
	// lower > upper is the wrap around case
	if ((lower <= wind_->get_direction() and wind_->get_direction() <= upper)
		or (lower > upper ) and (lower <= wind_->get_direction() or wind_->get_direction() <= upper)) {
		
		// if it does then apply the wind speed as a proportion of the disnace
		float distance = std::fmod(std::abs(wind_->get_direction() - direction_), PI2);
		if (distance > PI) distance = PI2 - distance; // Normalize distance to [0, π]

		// Calculate the proportion of the wind speed to apply the distance between the wind and sail as a proportion 
		// of the total it could be 
		auto proportion = 1.0f - (distance / (length_ / 2));
		force_ = Vector3 { wind_->get_speed() * proportion, 0.0f, wind_->get_speed() * proportion };
	}
	else {
		force_ = Vector3{( NO_WIND / 2), 0.0, (NO_WIND / 2 )};
	}

	// sail length, the longer the sail, the greater the speed
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
		force_coefficient_ = Vector3{ 1.0f, 0.0f, 1.0f };
	}
	else if (depth_ == ANCHOR_MAX_DEPTH) {
		force_coefficient_ = Vector3{ 0.0f, 0.0f, 0.0f };
	}
	else {
		// a function of the depth as the depth increases
		// the force moves closer to 0
		// the numbers might change
		auto depth_ratio = depth_ / ANCHOR_MAX_DEPTH;
		force_coefficient_ = Vector3{ 1.0f * depth_ratio, 0.0f, 1.0f / depth_ratio };
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
