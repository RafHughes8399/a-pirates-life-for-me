#include "object.h"

void Object::update(float delta) {
	// default does nothing
	return;
}

void Object::render(){
	// simply draws the object

	// for now only ocean but will change later when I 
	DrawModel(object_type_.get_model(), position_, 1.0f, BLUE);
	return;
}

void Object::interact(Object* other){
	return;
}

float Object::get_height(){
	auto bounding_box =  GetModelBoundingBox(object_type_.get_model());
	return std::abs(bounding_box.max.y - bounding_box.min.y);
}

float Object::get_width() {
	auto bounding_box = GetModelBoundingBox(object_type_.get_model());
	return std::abs(bounding_box.max.z - bounding_box.min.z);
}

float Object::get_length() {
	auto bounding_box = GetModelBoundingBox(object_type_.get_model());
	return std::abs(bounding_box.max.x - bounding_box.min.x);
}

Model& Object::get_model(){
	return object_type_.get_model();
}

Vector3 Object::get_position(){
	return position_;
}
BoundingBox& Object::get_bounding_box(){
	return bounding_box_;
}

int Object::get_id(){
	return id_;
}
void MoveableObject::update(float delta) {
	(void) delta;
	return;
}

void MoveableObject::render(){
	// do nothing i think maybe not 
	return;
}

Vector3 MoveableObject::get_acceleration(){
	return acceleration_;
}

Vector3 MoveableObject::get_velocity(){
	return velocity_;
}

float MoveableObject::get_direction(){
	return direction_;
}

Vector3 MoveableObject::get_direction_coefficient(){
	// cos on the z, sin on the x
	return Vector3{sinf(direction_), 1.0f, cosf(direction_)};
}

void MoveableObject::adjust_acceleration(Vector3 acceleration){
	acceleration_ = Vector3Add(acceleration_, acceleration);
}
