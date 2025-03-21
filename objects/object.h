#pragma once
#include <utility>
#include <string>
#include <cmath>
#include <iostream>

#include "raylib.h"
#include "raymath.h"

#include "ship_components.h"
class Object {
public:
	Object(Vector3 position, Vector3 size, Model model, float density, float volume)
		: position_(position), model_(model), size_(size), density_(density), volume_(volume){
		// generate the bounding box, min and max
		update_bounding_box();
	};

	Object(const Object& other)
		: position_(other.position_), size_(other.size_), model_(other.model_), bounding_box_(other.bounding_box_), density_(other.density_), volume_(other.volume_){
	};

	Object(Object&& other)
		: position_(std::move(other.position_)), size_(std::move(other.size_)), model_(std::move(other.model_)), bounding_box_(std::move(other.bounding_box_)), density_(std::move(other.density_)),
		volume_(std::move(other.volume_)) {
	};


	// a default update and render, the default update does nothing, render just draws the 
	virtual void update();
	virtual void render();
	virtual void interact(Object* other);
	float get_height();
	float get_width();
	float get_length();

	// getters and setters 
	Model& get_model();
	float get_density();
	Vector3 get_position();
	Vector3 get_size();
	BoundingBox get_bounding_box();
	void update_bounding_box();
	// operator overloads
protected:

	// all objects have a model and position, and a hitbox
	Model model_;
	Vector3 position_;
	Vector3 size_;
	BoundingBox bounding_box_;
	//Hitbox hitbox_;

	// objects also have 
	float density_;
	float volume_;
};

class MoveableObject : public Object {
public:
	MoveableObject(Vector3 position, Vector3 size, Model model, float density, float volume, Vector3 velocity, float direction)
		: Object(position, size, model, density, volume), velocity_(velocity), direction_(direction) {
		acceleration_ = Vector3{ 0.0,0.0,0.0 };
	};
	MoveableObject(const MoveableObject& other)
		: Object(other), velocity_(other.velocity_), acceleration_(other.acceleration_), direction_(other.direction_){
	};

	MoveableObject(MoveableObject&& other)
		: Object(other), velocity_(std::move(other.velocity_)), acceleration_(std::move(other.acceleration_)), direction_(std::move(other.direction_)) {
	}
	void update() override;
	void render() override;

	Vector3 get_acceleration();
	Vector3 get_velocity();
	float get_direction();
	Vector3 get_direction_coefficient();
	void adjust_acceleration(Vector3 acceleration);
	

protected:
	Vector3 velocity_;
	Vector3 acceleration_;
	float direction_; // in radians
};





class Ship : public MoveableObject {
public:

	Ship(Vector3 position, Vector3 size,  Model model, float density, float volume, Vector3 velocity, float direction, Wind* wind)
		: MoveableObject(position, size, model, density, volume, velocity, direction), sail_(Sail(direction, 4.2f, wind)), anchor_(Anchor()){
	};

	Ship(const Ship& other)
		: MoveableObject(other), sail_(other.sail_), anchor_(other.anchor_){

	};

	Ship(Ship&& other)
		: MoveableObject(other), sail_(std::move(other.sail_)), anchor_(std::move(other.anchor_)){
	};

	Ship& operator=(const Ship& ohter);
	Ship& operator= (const Ship&& other);

	void update() override;
	void render() override;
	void interact(Object* other) override;
	void set_position(Vector3 position);


	Vector3 get_position();
	Sail get_sail();
	Anchor get_anchor();
	void move_anchor();
	void drop_anchor();
	void raise_anchor();

	void steer_left();
	void steer_right();

	void raise_sail();
	void lower_sail();

	void turn_sail_left();
	void turn_sail_right();
	void set_wind(Wind* wind);
private:
	Sail sail_;
	Anchor anchor_;

};


class Ocean : public Object {
public:
	Ocean(Vector3 position, Vector3 size, Model model, float density, float volume)
		: Object(position, size, model, density, volume){
	};

	Ocean(const Ocean& other)
		: Object(other){
	};

	Ocean(Ocean&& other)
		: Object(other){
	};
	void interact(Object* other) override;
private:
};