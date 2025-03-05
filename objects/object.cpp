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

void MoveableObject::update(){
	return;
}

void MoveableObject::render(){
	// do nothing i think maybe not 
	return;
}
