#include "entities.h"
// =============== ENTITY  =============== 
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

Vector3& entities::entity::get_position(){
	return position_;
}
BoundingBox& entities::entity::get_bounding_box(){
	return bounding_box_;
}

int entities::entity::get_id(){
	return id_;
}

//  =============== STATIC ENTITY  ===============
//TODO implement
void entities::static_entity::interact(entities::entity& other){
	// your choice is pointers and if statements or references and try catch 

	// entity is either static or moveable, so if one cast fails try the other
	// dynamic casting with references throws an exception, so try one and catch it to do the other ? 
	auto static_entity_ptr = dynamic_cast<entities::static_entity*>(&other);
	if(static_entity_ptr){
		 
	}
	else{
		auto moveable_entity_ptr = dynamic_cast<entities::moveable_entity*>(&other);
		moveable_entity_ptr->interact(*this);
	}
	return;
}
std::unique_ptr<entities::entity> entities::static_entity::clone(){
	return nullptr;
}
//  =============== MOVEABLE ENTITY  ===============

int entities::moveable_entity::update(float delta) {
	(void) delta;
	return entities::status_codes::nothing;
}

void entities::moveable_entity::render(){
	// do nothing i think maybe not 
	return;
}
void entities::moveable_entity::interact(entities::entity& other){
	// cast to static, 
	// consider the physics for interacting moveable objects (deflection and some energy is lost)
	// cast to moveable, handle both cases

	// physics time yippeeeeee
	auto static_entity_ptr = dynamic_cast<entities::static_entity*>(&other);
	if(static_entity_ptr){
		// TODO: implement logic
	}
	else{
		auto moveable_entity_ptr = dynamic_cast<entities::moveable_entity*>(&other);
		// TODO: implement logic
	}
	return;
}
std::unique_ptr<entities::entity> entities::moveable_entity::clone(){
	return nullptr;
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
float entities::moveable_entity::get_mass(){
	return mass_;
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
std::unique_ptr<entities::entity> entities::test_entity::clone(){
	return nullptr;
}
