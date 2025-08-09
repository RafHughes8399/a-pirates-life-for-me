#ifndef PLAYER_H
#define PLAYER_H
#include <map>
#include <functional>
#include <utility>

#include "entities.h"
#include "config.h"
#include "rendering.h"
#include "events_interface.h"
#include "controls.h"

#include "../lib/raylib/src/raylib.h"
#include "../lib/raylib/src/raymath.h"
#include "../lib/raylib/src/rcamera.h"


// ok I need to define a constant distance betwen the camera position and the 
// target, as the target (the ship) moves, so too must the camera. it must move the same amount 

namespace player{
	// class inventory{}
	
	//TODO: figure out a more elegant way to track the player_ship, maybe a mediator 
	// TODO: a control map for key and mouse inputs to functions in the game
	// a reference the player_ship's position 
	class player {
		public:
		~player() = default;
		player()
		:camera_(Camera3D{}), ship_(nullptr), camera_mode_(CAMERA_THIRD_PERSON),
		 camera_frustrum_(camera_, ASPECT_RATIO, FOV, NEAR, FAR), camera_target_distance_(Vector3Subtract(SHIP_START, CAMERA_START)){
			std::cout << "build player" << std::endl;
			camera_.position = CAMERA_START;
			camera_.target = TARGET_START;// the camera looks at the cube, slightly above sea level
			camera_.up = Vector3{ 0.0, 1.0, 0.0 }; // rotation toward target
			camera_.fovy = FOV;
			camera_.projection = CAMERA_PERSPECTIVE; // should be third person mode ? 
		}
		player(const player& other) = default;
		player(player&& other) = default;

		player& operator=(const player& other) = default;
		player& operator= (player&& other) = default;
			
		void update(float delta);
		void render();
			
		Camera3D& get_camera();
		void move_camera(int mode);
		entities::player_ship* get_ship();
		void set_ship(entities::player_ship* player_ship);

		rendering::frustrum& get_frustrum();
		private:
			void check_key_input(float delta);
			int camera_mode_;
			Camera3D camera_;
			const Vector3 camera_target_distance_;
			rendering::frustrum camera_frustrum_;
			entities::player_ship* ship_;
	};
	
	class test_player{
		public:
		~test_player() = default;
		test_player()
		:camera_(Camera3D{}),  camera_mode_(CAMERA_THIRD_PERSON),
		 camera_frustrum_(camera_, ASPECT_RATIO, FOV, NEAR, FAR){
			std::cout << "building test_player" << std::endl;
			camera_.position = Vector3Zero();;
			camera_.target = Vector3Zero(); // the camera looks at the cube, slightly above sea level
			camera_.up = Vector3{ 0.0, 1.0, 0.0 }; // rotation toward target
			camera_.fovy = FOV;
			camera_.projection = CAMERA_PERSPECTIVE; // should be third person mode ? 
		}
		test_player(const test_player& other)
			: camera_(other.camera_), camera_mode_(other.camera_mode_),
			camera_frustrum_(other.camera_frustrum_){
		};
				
		test_player(test_player&& other);

		test_player& operator=(const test_player& other) {
			camera_ = other.camera_;
			camera_mode_ = other.camera_mode_;
			camera_frustrum_ = other.camera_frustrum_;
			return *this;
		}
		
		void update(float delta);
		rendering::frustrum& get_frustrum();
		Camera3D& get_camera();
		private:
			Camera3D camera_;
			int camera_mode_;
			rendering::frustrum camera_frustrum_;
	};
}
#endif