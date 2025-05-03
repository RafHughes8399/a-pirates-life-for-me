#pragma once
#include <map>
#include <functional>
#include <utility>
#include "raylib.h"
#include "object.h"
#include "config.h"
#include <rcamera.h>
class Player {
public:
	~Player() = default;
	Player()
		:camera_(Camera3D{}), ship_(nullptr), camera_mode_(CAMERA_THIRD_PERSON), chunk_(std::make_pair(0,0)){
			camera_.position = Vector3{ 0.0, 5.0, 5.0 };
			camera_.target = Vector3{ 0.0,0.5,0.0 }; // the camera looks at the cube, slightly above sea level
			camera_.up = Vector3{ 0.0, 1.0, 0.0 }; // rotation toward target
			camera_.fovy = 90;
			camera_.projection = CAMERA_PERSPECTIVE; // should be third person mode 
			set_default_key_map();
	}
	Player(Ship* ship)
		:camera_(Camera3D{}), ship_(ship), camera_mode_(CAMERA_THIRD_PERSON), chunk_(std::make_pair(0,0)){
		camera_.position = Vector3{ 0.0, 5.0, 5.0 };
		camera_.target = Vector3{ 0.0,0.5,0.0 }; // the camera looks at the cube, slightly above sea level
		camera_.up = Vector3{ 0.0, 1.0, 0.0 }; // rotation toward target
		camera_.fovy = 90;
		camera_.projection = CAMERA_PERSPECTIVE; // should be third person mode ? 
		set_default_key_map();
	}	

	Player(const Player& other)
		: camera_(other.camera_), ship_(other.ship_), camera_mode_(other.camera_mode_), key_pressed_inputs_(other.key_pressed_inputs_),
		key_down_inputs_(other.key_down_inputs_), chunk_(other.chunk_){
	};

	Player(Player&& other);

	Player& operator=(const Player& other) {
		camera_ = other.camera_;
		ship_ = other.ship_;
		camera_mode_ = other.camera_mode_;
		chunk_ = other.chunk_;
	}
	Player& operator= (const Player&& other);

	void update(float delta);
	void render();

	Camera3D& get_camera();
	std::pair<int, int>& get_chunk();
	void move_camera(int mode, Vector3& new_position);
	Ship* get_ship();
	void set_ship(Ship* ship);
private:

	void check_key_input(float delta);
	void set_default_key_map();

	Camera3D camera_;
	int camera_mode_;
	
	std::pair <int, int> chunk_;
	Ship* ship_;


	std::map<int, std::function<void(float)>> key_down_inputs_;
	std::map<int, std::function<void()>> key_pressed_inputs_;
};