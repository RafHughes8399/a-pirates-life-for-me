#pragma once
#include "raylib.h"
#include "../objects/Ship.h"
class Player {
public:
	Player()
		:camera_(Camera3D{}), ship_(Ship()) {

		camera_.position = Vector3{ 0.0, 5.0, 5.0 };
		camera_.target = Vector3{ 0.0,0.5,0.0 }; // the camera looks at the cube, slightly above sea level
		camera_.up = Vector3{ 0.0, 1.0, 0.0 }; // rotation toward target
		camera_.fovy = 90;
		camera_.projection = CAMERA_PERSPECTIVE; // should be third person mode ? 
	}

	Player(const Player& other);

	Player(const Player&& other);

	Player& operator=(const Player& ohter);
	Player& operator = (const Player&& other);

	void update();
	void render();
private:
	Camera3D camera_;
	Ship ship_;
};