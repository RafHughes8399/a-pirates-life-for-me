#include "entities.h"

void entities::terrain::update(float delta){
	return;
}
void entities::terrain::render() {
	DrawModel(object_type_.get_model(), position_, 0.7, WHITE);
	DrawBoundingBox(bounding_box_, GREEN);
}

void entities::terrain::interact(entities::entity* other) {
	return;
}
