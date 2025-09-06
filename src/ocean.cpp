#include "entities.h"

void entities::ocean::interact(entities::entity& other) {
	(void) other;
/* 	auto player_ship = dynamic_cast<entities::player_ship*>(other);
	if (player_ship != nullptr) {
		auto buoynacy = Vector3{ 0.0f,0.0f,0.0f };

		auto submerged_height = std::abs(0.0f - player_ship->get_position().y); // this is what is making it crash out
		auto p = WATER_DENISTY;
		auto g = GRAVITY;
		auto v = player_ship->get_width() * player_ship->get_length() * submerged_height;
		buoynacy.y += p * g * v * -1;
		player_ship->adjust_acceleration(buoynacy);
	} */
	return;
}

void entities::ocean::render() {
	std::cout << "======RENDER OCEAN========" << std::endl;
	std::cout << IsModelValid(object_type_.get_model()) << std::endl;
	std::cout << bounding_box_.min.x << ", " << bounding_box_.min.y << ", " << bounding_box_.min.z << std::endl;
	std::cout << bounding_box_.max.x << ", " << bounding_box_.max.y << ", " << bounding_box_.max.z << std::endl;
	DrawModel(object_type_.get_model(), position_, 1.0f, Fade(BLUE, 0.55));
	DrawBoundingBox(bounding_box_, BLUE);

	std::cout << "======END RENDER OCEAN========" << std::endl;
}


