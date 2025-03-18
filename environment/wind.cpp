#include "wind.h"
#include <iostream>
float Wind::get_speed() const{
	return speed_;
}

float Wind::get_direction() const{
	return direction_;
}

void Wind::pick_speed(){
	auto mod = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	speed_ = mod * WIND_SPEED_MAX; // always a positive value
	if (speed_ < WIND_SPEED_MIN) { speed_ = WIND_SPEED_MIN; }
}

void Wind::pick_direction(){
	auto mod = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	direction_ = mod* PI2;
}

void Wind::update_time(double time){
	time_randomised_ = time;
}

void Wind::update(double time){
	if (std::fmod(time, time_randomised_) <= 0.25) {
		pick_direction();
		pick_speed();
	}
}

Vector3 Wind::get_direction_coefficient(){
	// cos on the z, sin on the x 
	return Vector3{sinf(direction_), 0.0f, sinf(direction_)};
}
