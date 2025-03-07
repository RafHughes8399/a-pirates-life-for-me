#pragma once
#include <utility>
#include "raylib.h"
class Object {
public:
	Object(Vector3 position, Model model, float density, float volume)
		: position_(position), model_(model), density_(density), volume_(volume){
	};

	Object(const Object& other)
		: position_(other.position_), model_(other.model_), density_(other.density_), volume_(other.volume_){
	};

	Object(const Object&& other)
		: position_(std::move(other.position_)), model_(std::move(other.model_)), density_(std::move(other.density_)),
		volume_(std::move(other.volume_)) {
	};


	// a default update and render, the default update does nothing, render just draws the 
	virtual void update();
	virtual void render();
protected:

	// all objects have a model and position, and a hitbox
	Model model_;
	Vector3 position_;

	//Hitbox hitbox_;

	// objects also have 
	float density_;
	float volume_;
};

class MoveableObject : public Object {
public:
	MoveableObject(Vector3 position, Model model, float density, float volume, Vector3 velocity)
		: Object(position, model, density, volume), velocity_(velocity) {
	};
	MoveableObject(const MoveableObject& other)
		: Object(other), velocity_(other.velocity_){
	};

	MoveableObject(const MoveableObject&& other)
		: Object(other), velocity_(std::move(other.velocity_)) {
	}
	void update() override;
	void render() override;

protected:
	Vector3 velocity_;
};

class Ship : public MoveableObject {
public:
	Ship(Vector3 position, Model model, float density, float volume, Vector3 velocity,
		Vector3 sail_direction, float sail_length)
		: MoveableObject(position, model, density, volume, velocity), sail_direction_(sail_direction),  sail_length_(sail_length){
	};

	Ship(const Ship& other)
		: MoveableObject(other), sail_direction_(other.sail_direction_), sail_length_(other.sail_length_), anchored_(other.anchored_){
	};

	Ship(const Ship&& other)
		: MoveableObject(other), sail_direction_(std::move(other.sail_direction_)), sail_length_(std::move(other.sail_length_)),
		anchored_(std::move(other.anchored_)) {
	};

	Ship& operator=(const Ship& ohter);
	Ship& operator = (const Ship&& other);

	void update() override;
	void render() override;

	void set_position(Vector3 position);
	Vector3 get_position();
	void drop_anchor();
	void raise_anchor();
private:
	Vector3 sail_direction_;
	float sail_length_;
	bool anchored_ = false;
};