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


void Sail::sail_left() {
	// Get the relative angle between sail and ship, normalized to [-π, π]
	float relative_angle = direction_ - *ship_direction_;
	relative_angle = std::fmod(relative_angle, PI2);
	if (relative_angle > PI) relative_angle -= PI2;
	if (relative_angle < -PI) relative_angle += PI2;

	// Try to turn left
	auto new_dir = direction_ + SAIL_TURN_SPEED;
	new_dir = std::fmod(new_dir, PI2);
	if (new_dir < 0) new_dir += PI2;

	// Calculate new relative angle
	float new_relative = new_dir - *ship_direction_;
	new_relative = std::fmod(new_relative, PI2);
	if (new_relative > PI) new_relative -= PI2;
	if (new_relative < -PI) new_relative += PI2;

	// Only update if within bounds
	if (new_relative <= PI / 2) {
		direction_ = new_dir;
	}
}

void Sail::sail_right() {
	// Get the relative angle between sail and ship, normalized to [-π, π]
	float relative_angle = direction_ - *ship_direction_;
	relative_angle = std::fmod(relative_angle, PI2);
	if (relative_angle > PI) relative_angle -= PI2;
	if (relative_angle < -PI) relative_angle += PI2;

	// Try to turn right
	auto new_dir = direction_ - SAIL_TURN_SPEED;
	new_dir = std::fmod(new_dir, PI2);
	if (new_dir < 0) new_dir += PI2;

	// Calculate new relative angle
	float new_relative = new_dir - *ship_direction_;
	new_relative = std::fmod(new_relative, PI2);
	if (new_relative > PI) new_relative -= PI2;
	if (new_relative < -PI) new_relative += PI2;

	// Only update if within bounds
	if (new_relative >= -PI / 2) {
		direction_ = new_dir;
	}
}

// these are for when the sail is being moved along with the ship
void Sail::sail_left(float rad){
	direction_ = std::fmod(direction_ + rad, 2 *PI);
}
void Sail::sail_right(float rad) {
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
	auto arc = length_ * direction_;
	auto upper = std::fmod(direction_ + (arc /2), PI2);
	auto lower = std::fmod(direction_ - (arc / 2), PI2);
	if (lower < 0) {
		direction_ += PI2;
	}
	// compare to wind, how close is it to the centre 
	if (lower <= wind_->get_direction() and wind_->get_direction() <= upper) {
		auto distance = std::abs(wind_->get_direction() - direction_);
		auto proportion = distance / (arc / 2);
		force_ = Vector3 { wind_->get_speed() * proportion, 0.0f, wind_->get_speed() * proportion };
	}
	else {
		force_ = Vector3{( WIND_SPEED_MIN / 2), 0.0, (WIND_SPEED_MIN / 2 )};
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
