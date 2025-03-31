#include "object.h"

void Object::update(){
	// default does nothing
	update_bounding_box();
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

std::unique_ptr<Object> Object::clone(){
	return std::make_unique<Object>(*this);
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

float Object::get_density(){
	return density_;
}

Vector3 Object::get_position(){
	return position_;
}

Vector3 Object::get_size(){
	return size_;
}

BoundingBox Object::get_bounding_box(){
	return bounding_box_;
}

void Object::update_bounding_box(){
	bounding_box_ = BoundingBox{ position_.x - (size_.x / 2), position_.y - (size_.y / 2), position_.z - (size_.z / 2),
								position_.x + (size_.x / 2), position_.y + (size_.y / 2), position_.z + (size_.z / 2) };
}

void MoveableObject::update(){
	return;
}

void MoveableObject::render(){
	// do nothing i think maybe not 
	return;
}

std::unique_ptr<Object> MoveableObject::clone()
{
	return std::make_unique<MoveableObject>(*this);
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
	return Vector3{sinf(direction_), 0.0f, cosf(direction_)};
}

void MoveableObject::adjust_acceleration(Vector3 acceleration){
	acceleration_ = Vector3Add(acceleration_, acceleration);
}

void Terrain::render(){
	DrawModel(object_type_.get_model(), position_, 1.0f, RED);
}

void Terrain::interact(Object* other){
	return;
}
