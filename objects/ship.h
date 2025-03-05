#pragma once
#include "raylib.h"

class Ship {
public:
	Ship()
		: position_(Vector3{ 0.0,0.0,0.0 }), ship_model_(LoadModelFromMesh(GenMeshCube(1.0f, 1.5f, 4.5f))),
		acceleration_(Vector3{ 0.0,0.0,0.0 }), velocity_(Vector3{ 0.0,0.0,0.0 }), mass_(50){
	};

	Ship(const Ship& other)
		: position_(other.position_), ship_model_(other.ship_model_), anchored_(other.anchored_) {
	};

	Ship(const Ship&& other);

	Ship& operator=(const Ship& ohter);
	Ship& operator = (const Ship&& other);

	void update();
	void render();



	void set_position(Vector3 position);
	Vector3 get_position();
	void drop_anchor();
	void raise_anchor();
private:

	Model ship_model_; // a simple box for now i think 
	Vector3 position_;
	Vector3 acceleration_;
	Vector3 velocity_;
	float volume_;
	float mass_;
	
	
	
	Vector3 sail_direction_;
	float sail_length_;
	bool anchored_ = false;
};