#include "player.h"
#include "config.h"
#include "raymath.h"
#include "rcamera.h"
// player is updated after the objects so this should wok
void Player::update(){
	auto ship_position = ship_->get_position();
	auto target_difference = Vector3Subtract(ship_position, camera_.target);
	
	
	move_camera(camera_mode_, target_difference);
	//UpdateCamera(&camera_, CAMERA_THIRD_PERSON);
}
void Player::render() {

	//ship_.render(); - the ship is rendered by the world
}

Camera3D Player::get_camera(){
	return camera_;
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
