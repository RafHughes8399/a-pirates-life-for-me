#pragma once
#include "raylib.h"

class Ship {
public:
	Ship();

	Ship(const Ship& other);

	Ship(const Ship&& other);

	Ship& operator=(const Ship& ohter);
	Ship& operator = (const Ship&& other);

	void update();
	void render();



	void set_position(Vector3 position);
private:

	Model ship_model_; // a simple box for now i think 
	Vector3 position_;
	
	float volume_;
	float mass_;
	float velocity_;
	
	
	Vector3 sail_direction_;
	float sail_length_;
};