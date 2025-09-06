#include "entities.h"

int entities::entity::update(float delta) {
	// default does nothing
	return entities::status_codes::nothing;
}

void entities::entity::render(){
	// simply draws the object

	// for now only ocean but will change later when I 
	DrawModel(object_type_.get_model(), position_, 1.0f, BLUE);
	return;
}

void entities::entity::interact(entities::entity& other){
	(void) other;
	return;
}

float entities::entity::get_height(){
	auto bounding_box =  GetModelBoundingBox(object_type_.get_model());
	return std::abs(bounding_box.max.y - bounding_box.min.y);
}

float entities::entity::get_width() {
	auto bounding_box = GetModelBoundingBox(object_type_.get_model());
	return std::abs(bounding_box.max.z - bounding_box.min.z);
}

float entities::entity::get_length() {
	auto bounding_box = GetModelBoundingBox(object_type_.get_model());
	return std::abs(bounding_box.max.x - bounding_box.min.x);
}

Model& entities::entity::get_model(){
	return object_type_.get_model();
}

Vector3 entities::entity::get_position(){
	return position_;
}
BoundingBox& entities::entity::get_bounding_box(){
	return bounding_box_;
}

int entities::entity::get_id(){
	return id_;
}
int entities::moveable_entity::update(float delta) {
	(void) delta;
	return entities::status_codes::nothing;
}

void entities::moveable_entity::render(){
	// do nothing i think maybe not 
	return;
}

Vector3 entities::moveable_entity::get_acceleration(){
	return acceleration_;
}

Vector3 entities::moveable_entity::get_velocity(){
	return velocity_;
}

float entities::moveable_entity::get_direction(){
	return direction_;
}

Vector3 entities::moveable_entity::get_direction_coefficient(){
	// cos on the z, sin on the x
	return Vector3{sinf(direction_), 1.0f, cosf(direction_)};
}

void entities::moveable_entity::adjust_acceleration(Vector3 acceleration){
	acceleration_ = Vector3Add(acceleration_, acceleration);
}


int entities::test_entity::update(float delta ){
	(void) delta;
	return entities::status_codes::nothing;
}

void entities::test_entity::render(){
	DrawModel(object_type_.get_model(), position_, 1.0f, RED);
	return;
}

void entities::test_entity::interact(entity& other){
	(void) other;
	return;
}
