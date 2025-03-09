#include "managers.h"

void Interaction_Manager::interact(Ship& ship, Ocean& ocean) {
	auto buoynacy = Vector3{ 0.0f,0.0f,0.0f };
	auto submerged_height = std::abs(0.0f - ship.get_position().y);
	auto p = ocean.get_density();
	auto g = GRAVITY;
	auto v = ship.get_width() * ship.get_length() * submerged_height;
	buoynacy.y += p * g * v * -1;

	ship.adjust_acceleration(buoynacy);
}

void Interaction_Manager::interact(Ocean& ocean, Ship& ship){
	return;
}
