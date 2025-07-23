#include "entities.h"

void entities::ocean::interact(entities::entity* other) {
	auto ship = dynamic_cast<entities::ship*>(other);
	if (ship != nullptr) {
		auto buoynacy = Vector3{ 0.0f,0.0f,0.0f };

		auto submerged_height = std::abs(0.0f - ship->get_position().y); // this is what is making it crash out
		auto p = WATER_DENISTY;
		auto g = GRAVITY;
		auto v = ship->get_width() * ship->get_length() * submerged_height;
		buoynacy.y += p * g * v * -1;
		ship->adjust_acceleration(buoynacy);
	}
}

void entities::ocean::render() {
	DrawModel(object_type_.get_model(), position_, 1.0f, Fade(BLUE, 0.75));
	DrawBoundingBox(bounding_box_, RED);
}


