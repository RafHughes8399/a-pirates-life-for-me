#include "entities.h"

void entities::ocean::interact(entities::entity& other) {
	(void) other;
/* 	auto player_ship = dynamic_cast<entities::player_ship*>(other);
	if (player_ship != nullptr) {
		
	auto submerged_height = std::abs(0.0f - player_ship->get_position().y); // this is what is making it crash out
	auto p = WATER_DENISTY;
	auto g = GRAVITY;
	auto v = player_ship->get_width() * player_ship->get_length() * submerged_height;
	buoynacy.y += p * g * v * -1;
	player_ship->adjust_acceleration(buoynacy);
	} */

// cast, 
// then if static do nothing ? idk let it sink maybe 
	// if movebale then determine sinking / floating
	
	// TODO buoyancy calculations
	auto buoynacy = Vector3{ 0.0f,0.0f,0.0f };
	auto static_entity_ptr = dynamic_cast<entities::static_entity*>(&other);
	if(static_entity_ptr){
		// sink that b
	}
	else{
		// try to sink that b
		auto moveable_entity_ptr = dynamic_cast<entities::moveable_entity*>(&other);

		//moveable_entity_ptr->change_position(buoyancy);
	}
	return;
}
std::unique_ptr<entities::entity> entities::ocean::clone(){
	return nullptr;
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


