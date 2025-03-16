#include "wind.h"
#include <iostream>
float Wind::get_speed(){
	return speed_;
}

float Wind::get_direction(){
	return direction_;
}

void Wind::pick_speed(){
	auto mod = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	speed_ = mod * WIND_SPEED_MAX; // always a positive value
}

void Wind::pick_direction(){
	auto mod = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	direction_ = mod* PI2;
}

void Wind::update_time(double time){
	time_randomised_ = time;
}

void Wind::update(double time){
	std::cout << time_randomised_ << " < " << time << std::endl;
	if (std::fmod(time_randomised_, time) == 0) { // never actually bang on the second, rethink
		std::cout << "update" << std::endl;
		pick_direction();
		pick_speed();
	}
}

Vector3 Wind::get_direction_coefficient(){
	// cos on the x, sin on the z 
	return Vector3{cosf(direction_), 0.0f, sinf(direction_)};
}
