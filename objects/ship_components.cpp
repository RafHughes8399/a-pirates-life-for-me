#include "../objects/ship_components.h"
float Sail::get_sail_direction(){
	return direction_;
}

float Sail::get_sail_length(){
	return length_;
}

float Sail::get_width(){
	return width_;
}

const Wind* Sail::get_wind(){
	return wind_;
}

float Sail::sail_arc(){
	return direction_ * width_;
}

void Sail::sail_left(){
	// >= pi, lock it at pi, 
	direction_ = std::min(std::numbers::pi_v<float>, direction_ + SAIL_TURN_SPEED);

}
void Sail::sail_right(){
	// <= 0, lock it at zero
	direction_ = std::max(0.0f, std::fmod(direction_ - SAIL_TURN_SPEED, std::numbers::pi_v<float>));
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

void Sail::calcualte_force(){
	// get the upper and lower bounds of the sail
	auto arc = length_ * direction_;
	auto upper = std::fmod(direction_ + (arc /2), PI2);
	auto lower = std::fmod(direction_ - (arc / 2), PI2);
	if (lower < 0) {
		direction_ += PI2;
	}
	// compare to wind 
	if (lower <= wind_->get_direction() and wind_->get_direction() <= upper) {
		force_ = { 2.5f, 0.0f, 2.5f }; // placeholder 
	}
	else {
		force_ = { 1.25f, 0.0f, 1.25f }; // placeholder
	}
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
		force_coefficient_ = Vector3{ 1.0f / depth_, 0.0f, 1.0f / depth_ };
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
