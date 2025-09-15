#include "entities.h"

void entities::ocean::interact(entities::entity& other) {
	// TODO buoyancy calculations
	/**
	 // ? so what is the logic for the buoyancy, buoyancy is the force exerted on the object
	Fb = pgv
	 	p fluid is the density of the fluid 
		V is the volume of displaced fluid 
		g is gravity (mult by -1 since grav is negative)
	 * 	? take the submerged height of the ship (compare the bb.min.y with SEA_LEVEL)
	 */
	auto static_entity_ptr = dynamic_cast<entities::static_entity*>(&other);
	if(static_entity_ptr){
		// sink that b
	}
	else{
		// try to sink that b
		auto moveable_entity_ptr = dynamic_cast<entities::moveable_entity*>(&other);
		auto buoyancy = 0.0f;
		auto other_entity_bounds = moveable_entity_ptr->get_bounding_box();
		
		auto length = other_entity_bounds.max.x - other_entity_bounds.min.x;
		auto width = other_entity_bounds.max.z - other_entity_bounds.min.z;
		/**
		 * if other_entity_bounds.min.y > 0, then SEA_LEVEL - it will be < 0, which
		 * would create a negative height suggesting that the entity is not submerged
		 * 
		 * otherwise if other_entity_bounds.min.y < 0 then SEA_LEVEL - it will be > 0, which 
		 * creates a positive height and allows for a calculation of the submerged volume of the entity
		 */
		auto submerged_height = std::max(0.0f, SEA_LEVEL - other_entity_bounds.min.y); // 0 means not submerged
		auto volume = length * width * submerged_height;
		if(volume){
			buoyancy = WATER_DENISTY * GRAVITY * volume * -1;
			buoyancy =  buoyancy / moveable_entity_ptr->get_mass();
			std::cout << "buoyancy:  " <<  buoyancy << std::endl;
			std::cout << "gravity:  " << GRAVITY <<  std::endl;
			moveable_entity_ptr->adjust_acceleration(Vector3{0.0f, buoyancy, 0.0f});
		}
	}
	return;
}
std::unique_ptr<entities::entity> entities::ocean::clone(){
	return nullptr;
}
void entities::ocean::render() {
	DrawModel(object_type_.get_model(), position_, 1.0f, Fade(BLUE, 0.55));
	DrawBoundingBox(bounding_box_, BLUE);
}


