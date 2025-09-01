#include "entities.h"

int entities::terrain::update(float delta){
	return entities::status_codes::nothing;
}
void entities::terrain::render() {
	DrawModel(object_type_.get_model(), position_, 0.7, WHITE);
	DrawBoundingBox(bounding_box_, GREEN);
}

void entities::terrain::interact(entities::entity* other) {
	return;
}
