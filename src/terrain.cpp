#include "entities.h"

int entities::terrain::update(float delta){
	return entities::status_codes::nothing;
}
void entities::terrain::render() {
	DrawModel(object_type_.get_model(), position_, 0.7, WHITE);
	DrawBoundingBox(bounding_box_, GREEN);
}

void entities::terrain::interact(entities::entity& other) {
	(void) other;
	auto static_entity_ptr = dynamic_cast<entities::static_entity*>(&other);
	if(static_entity_ptr){

	}
	else{
		auto moveable_entity_ptr = dynamic_cast<entities::moveable_entity*>(&other);
	}
	return;

}
std::unique_ptr<entities::entity> entities::terrain::clone(){
	return nullptr;
}