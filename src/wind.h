#pragma once
#include <utility>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include "object.h"
#include "config.h"

class Wind {
public:
	~Wind() = default;
	// default constructor
	Wind()
		: direction_(0.0f), speed_(0.0f){
	};
	Wind(float direction, float speed)
		:direction_(direction), speed_(speed) {
	};
	Wind(const Wind& other)
		: direction_(other.direction_), speed_(other.speed_), time_randomised_(other.time_randomised_),
		ship_subscribers_(other.ship_subscribers_){
	};
	Wind(Wind&& other)
		:direction_(std::move(other.direction_)), speed_(std::move(other.speed_)), time_randomised_(std::move(other.time_randomised_)) {
	};

	float get_speed() const;
	float get_direction() const;

	void pick_speed();
	void pick_direction();

	void update_time(double time);
	void update(double time);
	Vector3 get_direction_coefficient();

	void add_ship_subscriber(std::weak_ptr<Ship>& ship);
	void remove_ship_subscriber(std::weak_ptr<Ship>& ship);
	void notify_ships();
	void notify_ship(size_t ship_);

private:
	std::vector<std::weak_ptr<Ship>> ship_subscribers_;
	float direction_;
	float speed_; 
	double time_randomised_ = WIND_CHANGE_TIME;
};