#pragma once
#include <utility>
#include <string>
#include <cmath>
#include <iostream>

#include "singleton_flyweight.h"
#include "raylib.h"
#include "raymath.h"
#include "ship_components.h"
class Object {
public:
	Object(ObjectType& object_type, Vector3 position, Vector3 size, float density)
		: object_type_(object_type),position_(position), size_(size), density_(density), volume_(size.x * size.y * size.z){
		// generate the bounding box, min and max
		update_bounding_box();
	};

	Object(const Object& other)
		: object_type_(other.object_type_), position_(other.position_), size_(other.size_), bounding_box_(other.bounding_box_), density_(other.density_), volume_(other.volume_){
	};

	Object(Object&& other)
		: object_type_(other.object_type_), position_(std::move(other.position_)), size_(std::move(other.size_)), bounding_box_(std::move(other.bounding_box_)), density_(std::move(other.density_)),
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
	ObjectType& object_type_;
	Vector3 position_;
	
	Vector3 size_;
	BoundingBox bounding_box_;

	// objects also have 
	float density_;
	float volume_;
};

class MoveableObject : public Object {
public:
	MoveableObject(ObjectType& object_type, Vector3 position, Vector3 size, float density,  Vector3 velocity, float direction)
		: Object(object_type, position, size, density), velocity_(velocity), direction_(direction) {
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

	Ship(ShipType& ship_type, Vector3 position, Vector3 size, float density, Vector3 velocity, float direction)
		: MoveableObject(ship_type, position, size, density, velocity, direction), sail_(Sail(direction, 4.2f)), anchor_(Anchor()){

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

	void steer_left();
	void steer_right();

	void raise_sail();
	void lower_sail();

	void turn_sail_left();
	void turn_sail_right();
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
	Terrain(TerrainType& terrain_type, Vector3 position, Vector3 size, float density)
		: Object(terrain_type, position, size, density) {
		position_ = Vector3{size_.x * -0.5f, size_.y * -0.75f, size_.z * -0.5f};
		terrain_type.get_model().materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = terrain_type.get_texture();
	};
	Terrain(const Terrain& other)
		: Object(other) {
	};
	Terrain(Terrain&& other)
		: Object(other) {
	};
	

	void render() override;
	void interact(Object* other) override;
private:

};
