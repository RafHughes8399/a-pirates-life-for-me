#include "../objects/object.h"
#include "../game/config.h"

void Ocean::interact(Object* other) {
	auto ship = dynamic_cast<Ship*>(other);
	if (ship != nullptr) {
		//std::cout << " ocean interacts with the ship " << std::endl;
		auto buoynacy = Vector3{ 0.0f,0.0f,0.0f };

		auto submerged_height = std::abs(0.0f - ship->get_position().y); // this is what is making it crash out
		//std::cout << "submerged height is " << submerged_height << std::endl;
		auto p = get_density();
		auto g = GRAVITY;
		auto v = ship->get_width() * ship->get_length() * submerged_height;
		buoynacy.y += p * g * v * -1;
		//std::cout << buoynacy.x << ",  " <<  buoynacy.y  << ", "  << ", " << buoynacy.z << std::endl;
		ship->adjust_acceleration(buoynacy);
	}
}
