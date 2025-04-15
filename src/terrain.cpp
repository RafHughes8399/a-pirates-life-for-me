#include "object.h"


void Terrain::render() {
	DrawModel(object_type_.get_model(), position_, 1.0, WHITE);
}

void Terrain::interact(Object* other) {
	return;
}
