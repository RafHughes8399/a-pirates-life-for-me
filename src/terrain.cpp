#include "object.h"


void Terrain::render() {
	DrawModel(object_type_.get_model(), position_, 0.7, WHITE);
	DrawBoundingBox(bounding_box_, GREEN);
}

void Terrain::interact(Object* other) {
	return;
}
