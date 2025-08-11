#include "environment.h"
#include <iostream>
float environment::wind::get_speed() const{
	return speed_;
}

float environment::wind::get_direction() const{
	return direction_;
}

void environment::wind::pick_speed(){
	auto mod = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	speed_ = mod * WIND_SPEED_MAX; // always a positive value
	if (speed_ < WIND_SPEED_MIN) { speed_ = WIND_SPEED_MIN; }
}

void environment::wind::pick_direction(){
	auto mod = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	direction_ = mod* PI2;
}

void environment::wind::update_time(double time){
	time_randomised_ = time;
}

void environment::wind::update(double time){
	if (std::fmod(time, time_randomised_) <= 0.25) {
		pick_direction();
		pick_speed();
		notify_ships();
	}
}

Vector3 environment::wind::get_direction_coefficient(){
	// cos on the z, sin on the x 
	return Vector3{sinf(direction_), 0.0f, sinf(direction_)};
}

void environment::wind::add_ship_subscriber(entities::player_ship* & player_ship)
{
	ship_subscribers_.push_back(player_ship);
}

void environment::wind::remove_ship_subscriber(entities::player_ship* & player_ship){
	auto new_end = std::remove_if(ship_subscribers_.begin(), ship_subscribers_.end(), 
		[player_ship] (auto& subscriber) -> bool {
			return player_ship == subscriber;
		});
	ship_subscribers_.erase(new_end, ship_subscribers_.end());
}

void environment::wind::notify_ships(){
	for (auto & player_ship : ship_subscribers_) {
		player_ship->update_sail_wind(direction_, speed_);
	}
}

void environment::wind::notify_ship(size_t player_ship){
	if (player_ship < ship_subscribers_.size()) {
		ship_subscribers_[player_ship]->update_sail_wind(direction_, speed_);
	}
}
