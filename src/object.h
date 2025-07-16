#pragma once
#include <utility>
#include <string>
#include <cmath>
#include <iostream>

#include "singleton_flyweight.h"
#include "../lib/raylib/src/raylib.h"
#include "../lib/raylib/src/raymath.h"
#include "ship_components.h"
#include "config.h"

class Object {
public:
	Object(ObjectType& object_type, Vector3 position, Vector3 size, float density)
		: object_type_(object_type),position_(position), size_(size), density_(density), volume_(size.x * size.y * size.z){
		// generate the bounding box, min and max
		update_bounding_box();
	};	
	Object(ObjectType& object_type, Vector3 position, Vector3 size, Vector3 min, Vector3 max, float density)
		: object_type_(object_type),position_(position), size_(size), bounding_box_(BoundingBox{min, max}), density_(density), volume_(size.x * size.y * size.z){
		// generate the bounding box, min and max
	};

	Object(const Object& other)
		: object_type_(other.object_type_), position_(other.position_), size_(other.size_), bounding_box_(other.bounding_box_), density_(other.density_), volume_(other.volume_){
	};

	Object(Object&& other)
		: object_type_(other.object_type_), position_(std::move(other.position_)), size_(std::move(other.size_)), bounding_box_(std::move(other.bounding_box_)), density_(std::move(other.density_)),
		volume_(std::move(other.volume_)) {
	};


	// a default update and render, the default update does nothing, render just draws the 
	virtual void update(float delta);
	virtual void render();
	virtual void interact(Object* other);
	float get_height();
	float get_width();
	float get_length();

	int get_id();

	// getters and setters 
	Model& get_model();
	float get_density();
	Vector3 get_position();
	Vector3 get_size();
	BoundingBox get_bounding_box();
	void update_bounding_box();

	// operator overloads
	friend bool operator==(const Object& a, const Object& b){
		return a.id_ == b.id_;
	}
protected:

	// all objects have a model and position, and a hitbox
	ObjectType& object_type_;
	Vector3 position_;
	
	Vector3 size_;
	BoundingBox bounding_box_;

	// objects also have 
	float density_;
	float volume_;
	int id_;
};

class MoveableObject : public Object {
public:
	MoveableObject(ObjectType& object_type, Vector3 position, Vector3 size, float density,  Vector3 velocity, float direction)
		: Object(object_type, position, size, density), velocity_(velocity), direction_(direction) {
		acceleration_ = Vector3{ 0.0,0.0,0.0 };
	};	
	MoveableObject(ObjectType& object_type, Vector3 position, Vector3 size, Vector3 min, Vector3 max, float density,  Vector3 velocity, float direction)
		: Object(object_type, position, size, min, max,  density), velocity_(velocity), direction_(direction) {
		acceleration_ = Vector3{ 0.0,0.0,0.0 };
	};
	MoveableObject(const MoveableObject& other)
		: Object(other), velocity_(other.velocity_), acceleration_(other.acceleration_), direction_(other.direction_){
	};

	MoveableObject(MoveableObject&& other)
		: Object(other), velocity_(std::move(other.velocity_)), acceleration_(std::move(other.acceleration_)), direction_(std::move(other.direction_)) {
	}
	void update(float delta) override;
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

	Ship(ShipType& ship_type, Vector3 position, Vector3 size, float density, Vector3 velocity, float direction)
		: MoveableObject(ship_type, position, size, density, velocity, direction), sail_(Sail(direction, 4.2f)), anchor_(Anchor()){

	};	
	Ship(ShipType& ship_type, Vector3 position, Vector3 size, Vector3 min, Vector3 max, float density, Vector3 velocity, float direction)
		: MoveableObject(ship_type, position, size, min, max,  density, velocity, direction), sail_(Sail(direction, 4.2f)), anchor_(Anchor()){

	};

	Ship(const Ship& other)
		: MoveableObject(other), sail_(other.sail_), anchor_(other.anchor_){

	};

	Ship(Ship&& other)
		: MoveableObject(other), sail_(std::move(other.sail_)), anchor_(std::move(other.anchor_)){
	};

	Ship& operator=(const Ship& ohter);
	Ship& operator= (const Ship&& other);

	void update(float delta) override;
	void render() override;
	void interact(Object* other) override;
	void set_position(Vector3 position);


	Vector3 get_position();
	Sail get_sail();
	Anchor get_anchor();
	void move_anchor();

	void steer_left(float delta);
	void steer_right(float delta);

	void raise_sail(float delta);
	void lower_sail(float delta);

	void turn_sail_left(float delta);
	void turn_sail_right(float delta);
	void update_sail_wind(float direction, float speed);
private:
	Sail sail_;
	Anchor anchor_;

};


class Ocean : public Object {
public:
	Ocean(OceanType& ocean_type, Vector3 position, Vector3 size, float density)
		: Object(ocean_type, position, size, density){
	};

	Ocean(const Ocean& other)
		: Object(other){
	};

	Ocean(Ocean&& other)
		: Object(other){
	};
	void interact(Object* other) override;
	void render() override;
private:
};

class Terrain : public Object {
public:
	Terrain(ObjectType& terrain_type, Vector3 position, Vector3 size, float density)
		: Object(terrain_type, position, size, density) {
	};	
	Terrain(ObjectType& terrain_type, Vector3 position, Vector3 size, Vector3 min, Vector3 max, float density)
		: Object(terrain_type, position, size, min, max, density) {
	};
	Terrain(const Terrain& other)
		: Object(other) {
	};
	Terrain(Terrain&& other)
		: Object(other) {
	};
	
	void update(float delta) override;
	void render() override;
	void interact(Object* other) override;
private:

};
