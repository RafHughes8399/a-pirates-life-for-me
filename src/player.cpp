#include "player.h"
#include "config.h"
#include "utility_functions.h"
#include "raymath.h"
#include "rcamera.h"

// player is updated after the objects so this should wok
void Player::update(){
	check_key_input();
	auto ship_position = ship_->get_position();
	auto target_difference = Vector3Subtract(ship_position, camera_.target);
	
	move_camera(camera_mode_, target_difference);
	chunk_ = utility::position_to_chunk(ship_position);
	
}
void Player::render() {
	//this is where the hud will be drawn and other components
}

Camera3D Player::get_camera(){
	return camera_;
}

std::pair<int, int>& Player::get_chunk(){
	return chunk_;
}

void Player::move_camera(int mode, Vector3& difference){
	Vector2 delta_mouse = GetMouseDelta();
	auto delta_time = GetFrameTime();
	// my version of camera update

	// bool values for movement and rotation based on camera mode
	bool move_in_world_plane = ((mode == CAMERA_FIRST_PERSON) || (mode == CAMERA_THIRD_PERSON));
	bool rotate_around_target = ((mode == CAMERA_THIRD_PERSON) || (mode == CAMERA_ORBITAL));
	bool lock_view = ((mode == CAMERA_FREE) || (mode == CAMERA_FIRST_PERSON) || (mode == CAMERA_THIRD_PERSON) || (mode == CAMERA_ORBITAL));
	bool rotate_up = false;

	// camera movement properties
	float camera_move_speed = CAMERA_MOVE_SPEED * delta_time;
	float camera_rotation_speed = CAMERA_ROTATION_SPEED * delta_time;
	float camera_pan_speed = CAMERA_PAN_SPEED * delta_time;
	float camera_orbital_speed = CAMERA_ORBITAL_SPEED * delta_time;

	// move the camera to the new position
	camera_.position = Vector3Add(camera_.position, difference);
	camera_.target = Vector3Add(camera_.target, difference);

	// stop the camera from moving below the water level
	camera_.position.y = std::max(camera_.position.y, CAMERA_MIN_LEVEL);
	// then allow for camera rotation
	CameraYaw(&camera_, -delta_mouse.x * CAMERA_MOUSE_MOVE_SENSITIVITY, rotate_around_target);
	CameraPitch(&camera_, -delta_mouse.y * CAMERA_MOUSE_MOVE_SENSITIVITY, lock_view, rotate_around_target, rotate_up);
}

Ship* Player::get_ship(){
	return ship_;
}

void Player::set_ship(Ship* ship){
	ship_ = ship;
}

void Player::check_key_input(){
	// iterate through both key maps


	for (auto& input : key_down_inputs_) {
		if (IsKeyDown(input.first)) {
			input.second();
		}
	}
	for (auto& input : key_pressed_inputs_) {
		if (IsKeyPressed(input.first)) {
			input.second();
		}
	}
}

void Player::set_default_key_map(){
	// fill the control maps, figure out how to e

	key_down_inputs_[KEY_A] = [this]() { ship_->steer_left(); };
	key_down_inputs_[KEY_D] = [this]() { ship_->steer_right(); };


	key_down_inputs_[KEY_W] = [this]() { ship_->raise_sail(); };
	key_down_inputs_[KEY_S] = [this]() { ship_->lower_sail(); };

	key_down_inputs_[KEY_Q] = [this]() { ship_->turn_sail_left(); };
	key_down_inputs_[KEY_E] = [this]() { ship_->turn_sail_right(); };

	key_pressed_inputs_[KEY_R] = [this]() { ship_->move_anchor(); }; // adjust the function call, it should not be raise/lower anchor but rather 
	// something that changes the movement of the anchor . this is placeholder
}
