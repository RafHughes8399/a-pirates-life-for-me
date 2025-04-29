#include "object.h"
#include "config.h"

void Ocean::interact(Object* other) {
	auto ship = dynamic_cast<Ship*>(other);
	if (ship != nullptr) {
		auto buoynacy = Vector3{ 0.0f,0.0f,0.0f };

		auto submerged_height = std::abs(0.0f - ship->get_position().y); // this is what is making it crash out
		auto p = get_density();
		auto g = GRAVITY;
		auto v = ship->get_width() * ship->get_length() * submerged_height;
		buoynacy.y += p * g * v * -1;
		ship->adjust_acceleration(buoynacy);
	}
}

void Ocean::render() {
	DrawModel(object_type_.get_model(), position_, 1.0f, Fade(BLUE, 0.75));
	DrawBoundingBox(bounding_box_, RED);
}
