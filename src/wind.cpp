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
		notify_ships();
	}
}

Vector3 Wind::get_direction_coefficient(){
	// cos on the z, sin on the x 
	return Vector3{sinf(direction_), 0.0f, sinf(direction_)};
}

void Wind::add_ship_subscriber(std::weak_ptr<Ship>& ship)
{
	ship_subscribers_.push_back(ship);
}

void Wind::remove_ship_subscriber(std::weak_ptr<Ship>& ship){
	ship_subscribers_.erase(std::remove_if(ship_subscribers_.begin(), ship_subscribers_.end(),
		[&ship](const std::weak_ptr<Ship>& s) { return s.lock() == ship.lock(); }), ship_subscribers_.end());
}

void Wind::notify_ships(){
	for (auto & ship : ship_subscribers_) {
		if (auto s = ship.lock()) {
			s->update_sail_wind(direction_, speed_);
		}
	}
}

void Wind::notify_ship(size_t ship){
	if (ship < ship_subscribers_.size()) {
		ship_subscribers_[ship].lock()->update_sail_wind(direction_, speed_);
	}
}
