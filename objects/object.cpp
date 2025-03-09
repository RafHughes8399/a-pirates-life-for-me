#include "object.h"

void Object::update(){
	// default does nothing
	return;
}

void Object::render(){
	// simply draws the object

	// for now only ocean but will change later when I 
	DrawModel(model_, position_, 1.0f, BLUE);
	return;
}

void Object::interact(Object* other){
	return;
}

float Object::get_height(){
	auto bounding_box =  GetModelBoundingBox(model_);
	return std::abs(bounding_box.max.y - bounding_box.min.y);
}

float Object::get_width() {
	auto bounding_box = GetModelBoundingBox(model_);
	return std::abs(bounding_box.max.z - bounding_box.min.z);
}

float Object::get_length() {
	auto bounding_box = GetModelBoundingBox(model_);
	return std::abs(bounding_box.max.x - bounding_box.min.x);
}

Model& Object::get_model(){
	return model_;
}

float Object::get_density(){
	return density_;
}

void MoveableObject::update(){
	return;
}

void MoveableObject::render(){
	// do nothing i think maybe not 
	return;
}

Vector3 MoveableObject::get_acceleration(){
	return acceleration_;
}

void MoveableObject::adjust_acceleration(Vector3 acceleration){
	acceleration_ = Vector3Add(acceleration_, acceleration);
}





