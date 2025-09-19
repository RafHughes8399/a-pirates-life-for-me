#include "player.h"


void player::player::build_huds(){
	auto pirate_builder = pirate_hud_builder();
	auto ship_builder = ship_hud_builder();


	huds_[huds::pirate] = hud_director::build_hud(pirate_builder);
	huds_[huds::ship] = hud_director::build_hud(ship_builder);
}
// player is updated after the objects so this should wok
void player::player::update(float delta) {
	// check for key inputs, generate any events if they are pressed (or held down)
	check_key_input(delta);
	move_camera(camera_mode_);

}
void player::player::render() {
	//this is where the hud will be drawn and other components
	huds_[hud_index_].draw();
}

Camera3D& player::player::get_camera(){
	return camera_;
}

rendering::frustrum& player::player::get_frustrum(){
	return camera_frustrum_;
}

void player::player::move_camera(int mode){
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
	// stop the camera from moving below the water level
	// clamp across all directions
	camera_.position.y = Clamp(camera_.position.y, 0.5, WORLD_Y / 2);

	// then allow for camera rotation
	CameraYaw(&camera_, -delta_mouse.x * CAMERA_MOUSE_MOVE_SENSITIVITY, rotate_around_target);
	CameraPitch(&camera_, -delta_mouse.y * CAMERA_MOUSE_MOVE_SENSITIVITY, lock_view, rotate_around_target, rotate_up);
		
	camera_frustrum_.update_frustrum(camera_, ASPECT_RATIO, FOV, NEAR, FAR);	

}
void player::player::check_key_input(float delta){
	// iterate through both key maps
	for(auto& input : controls::ship_controls::get_instance().get_controls()){
		if(IsKeyDown(input)){
			std::unique_ptr<events::event> key_down = std::make_unique<events::player_input_event>(input);
			events::global_dispatcher_.queue_event(key_down);
		}
	}
}

void player::player::on_camera_move_event(const events::camera_move_event& event){
	// the vector tells how much the ship position has changed,
	// manipulate both the camera position and target
	// then recalculate the frustrum
	camera_.position = Vector3Add(camera_.position, event.get_position());
	camera_.target = Vector3Add(camera_.target, event.get_position());
	camera_frustrum_.update_frustrum(camera_, ASPECT_RATIO, camera_.fovy, NEAR, FAR);
	return;
}


void player::test_player::update(float delta){
	// allows the camera to move freely
	UpdateCamera(&camera_, CAMERA_FREE);

	// update the frustrum too
	camera_frustrum_.update_frustrum(camera_, ASPECT_RATIO, FOV, NEAR, FAR);
}

rendering::frustrum& player::test_player::get_frustrum(){
	return camera_frustrum_;
}

Camera3D& player::test_player::get_camera(){
	return camera_;
}