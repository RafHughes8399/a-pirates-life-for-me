#ifndef PLAYER_H
#define PLAYER_H
#include <map>
#include <functional>
#include <utility>

#include "entities.h"
#include "config.h"
#include "rendering.h"

#include "../lib/raylib/src/raylib.h"
#include "../lib/raylib/src/raymath.h"
#include "../lib/raylib/src/rcamera.h"
namespace player{
	// class inventory{}

	
	//TODO: figure out a more elegant way to track the ship, maybe a mediator 
	// TODO: a control map for key and mouse inputs to functions in the game
	class control_scheme{
	public:
		// TODO: methods that can be used by the event manager
	private:
		// std::map<int, event ?>
		// or maybe std::map<int, std::function>
	};
	// a reference the ship's position 
	class player {
		public:
		~player() = default;
		player()
		:camera_(Camera3D{}), ship_(nullptr), camera_mode_(CAMERA_THIRD_PERSON),
		 camera_frustrum_(camera_, ASPECT_RATIO, FOV, NEAR, FAR){
			std::cout << "building player" << std::endl;
			camera_.position = Vector3{ 0.0, 5.0, 5.0 };
			camera_.target = SHIP_START; // the camera looks at the cube, slightly above sea level
			camera_.up = Vector3{ 0.0, 1.0, 0.0 }; // rotation toward target
			camera_.fovy = FOV;
			camera_.projection = CAMERA_PERSPECTIVE; // should be third person mode ? 
			set_default_key_map();
		}
		player(const player& other)
			: camera_(other.camera_), ship_(other.ship_), camera_mode_(other.camera_mode_), key_pressed_inputs_(other.key_pressed_inputs_),
			key_down_inputs_(other.key_down_inputs_), camera_frustrum_(other.camera_frustrum_){
		};
				
		player(player&& other);

		player& operator=(const player& other) {
			camera_ = other.camera_;
			ship_ = other.ship_;
			camera_mode_ = other.camera_mode_;
			camera_frustrum_ = other.camera_frustrum_;
			return *this;
		}
		player& operator= (const player&& other);
			
		void update(float delta);
		void render();
			
		Camera3D& get_camera();
		std::pair<int, int>& get_chunk();
		void move_camera(int mode, Vector3& new_position);
		entities::ship* get_ship();
		void set_ship(entities::ship* ship);

		rendering::frustrum& get_frustrum();
		private:
			void check_key_input(float delta);
			void set_default_key_map();
			Camera3D camera_;
			rendering::frustrum camera_frustrum_;
			int camera_mode_;
			
			entities::ship* ship_;
		
			std::map<int, std::function<void(float)>> key_down_inputs_;
			std::map<int, std::function<void()>> key_pressed_inputs_;
	};
	
	class test_player{
		public:
		~test_player() = default;
		test_player()
		:camera_(Camera3D{}),  camera_mode_(CAMERA_THIRD_PERSON),
		 camera_frustrum_(camera_, ASPECT_RATIO, FOV, NEAR, FAR){
			std::cout << "building test_player" << std::endl;
			camera_.position = Vector3{ 0.0, 5.0, 5.0 };
			camera_.target = SHIP_START; // the camera looks at the cube, slightly above sea level
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