#include "entities.h"
#include "config.h"
void entities::player_ship::update(float delta){
	//MoveableObject::update(delta);
	// update the anchor
	anchor_.update();
	// apply gravity
	
	acceleration_.y += GRAVITY;

	// this calculates the veloctuty for a given frame
	// apply the sail movement coeffieicts
	DrawRectangle(800, 5, 195, 170, Fade(SKYBLUE, 0.5f));
	DrawRectangleLines(800, 5, 195, 170, BLUE);
	DrawText(TextFormat("Acceleration after gravity and buoyancy: (%06.3f, %06.3f, %06.3f)", acceleration_.x, acceleration_.y, acceleration_.z), 810, 30, 10, BLACK);

	auto sail_force = sail_.get_force();
	acceleration_ = Vector3Add(acceleration_, sail_.get_force());
	DrawText(TextFormat("Sail Force: (%06.3f, %06.3f, %06.3f)", sail_force.x, sail_force.y, sail_force.z), 810, 45, 10, BLACK);
	
	// apply the anchor force coefficient 
	auto anchor_force = anchor_.get_force();
	acceleration_ = Vector3Multiply(acceleration_, anchor_.get_force());
	DrawText(TextFormat("Anchor Force: (%06.3f, %06.3f, %06.3f)", anchor_force.x, anchor_force.y, anchor_force.z), 810, 60, 10, BLACK);
	
	// apply acceleration to velocity
	velocity_ = acceleration_;
	DrawText(TextFormat("Velocity Post Delta: (%06.3f, %06.3f, %06.3f)", velocity_.x, velocity_.y, velocity_.z), 810, 90, 10, BLACK);


	// apply the player_ship direction to the velocity, use sin and cos, other way around z is cos, sin is x
	auto direction_coefficient = get_direction_coefficient();
	velocity_ = Vector3Multiply(velocity_, direction_coefficient);
	DrawText(TextFormat("Direction Coefficient: (%06.3f, %06.3f, %06.3f)", direction_coefficient.x, direction_coefficient.y, direction_coefficient.z), 810, 105, 10, BLACK);
	DrawText(TextFormat("Velocity Post Direction: (%06.3f, %06.3f, %06.3f)", velocity_.x, velocity_.y, velocity_.z), 810, 130, 10, BLACK);
	
	// update pos
	velocity_ = Vector3Scale(velocity_, delta);
	position_ = Vector3Add(position_, velocity_);

	
	// reset accel 
	acceleration_ = Vector3Zero();

	// update the bounding box 
	bounding_box_.min = Vector3Add(bounding_box_.min, velocity_);
	bounding_box_.max = Vector3Add(bounding_box_.max, velocity_);
}

void entities::player_ship::render(){
	DrawModel(object_type_.get_model(), position_, 0.15f, WHITE);
	DrawBoundingBox(bounding_box_, RED);
}


void entities::player_ship::interact(entities::entity* other){
	auto ocean = dynamic_cast<entities::ocean*>(other);
	// cast to ocean
	if (ocean != nullptr) {
		
		// buoyancy
		auto buoynacy = Vector3{ 0.0f,-0.2f,0.0f };
		auto submerged_height = 0.2f;
		auto p = WATER_DENISTY;
		auto g = GRAVITY;
		auto v = get_width() * get_length() * submerged_height;
		buoynacy.y += p * g * v * -1; // make it a better deeper
		adjust_acceleration(buoynacy);
	}
	return;

}

void entities::player_ship::set_position(Vector3 position) {
	position_ = position;
}

Vector3 entities::player_ship::get_position(){
	return position_;
}

components::sail entities::player_ship::get_sail(){
	return sail_;
}

components::anchor entities::player_ship::get_anchor(){
	return anchor_;
}


void entities::player_ship::on_player_input_event(const events::player_input_event& event){
	// first print the key info
	std::cout << event.get_key() << " pressed ya heard" << std::endl;
	// i need a delta
	// something something 
	// ship_controls[key]
	// for controsl
	auto delta = GetFrameTime();
	auto ship_control = control_map_[event.get_key()];
	ship_control(delta);
}
void entities::player_ship::move_anchor(){
	// either drop or raise depending on the state
	// change the anchor stuff to move 
	anchor_.move();
}
//TODO: implement
void entities::player_ship::steer_ship(float delta, int direction){
	(void) delta;
	(void) direction;
	return;
}

void entities::player_ship::move_sail(float delta, int direction){
	(void) delta;
	(void) direction;
	return;
}

void entities::player_ship::move_anchor(float delta, int direction){
	(void) delta;
	(void) direction;
	return;
}
void entities::player_ship::steer_left(float delta){
	auto turn = SHIP_TURN_SPEED * delta;
	direction_ = std::fmod((direction_ + turn), ( 2 * PI));
	
	// rotate the model, yaw
	sail_.move_sail_left(turn);

	Vector3 rotate = { 0.0f, direction_, 0.0f };
	object_type_.get_model().transform = MatrixRotateXYZ(rotate);
}

void entities::player_ship::steer_right(float delta){
	// change player_ship direction to the right
	auto turn = SHIP_TURN_SPEED * delta;
	direction_ = std::fmod((direction_ - turn), ( 2 * PI));
	if (direction_ < 0.0) { direction_ += 2 * PI; }

	sail_.move_sail_right(turn);
	
	Vector3 rotate = {  0.0f,  direction_,  0.0f };
	object_type_.get_model().transform = MatrixRotateXYZ(rotate);
}

void entities::player_ship::raise_sail(float delta){
	auto raise = LOWER_RAISE_SPEED * delta;
	sail_.raise_sail(raise);
}

void entities::player_ship::lower_sail(float delta){
	auto lower = LOWER_RAISE_SPEED * delta;
	sail_.lower_sail(lower);
}

void entities::player_ship::turn_sail_left(float delta){
	sail_.sail_left(direction_, delta);
}

void entities::player_ship::turn_sail_right(float delta) {
	sail_.sail_right(direction_, delta);
}

void entities::player_ship::update_sail_wind(float direction, float speed){

	sail_.set_wind(direction, speed);

}


void entities::player_ship::init_control_map(){
	auto control_list = controls::ship_controls::get_instance().get_controls();
	control_map_[control_list[TURN_LEFT]] = [this](float delta){
			steer_ship(delta, 1);
	};
	control_map_[control_list[TURN_RIGHT]] = [this](float delta){
		steer_ship(delta, - 1);
	};
	// TODO refactor sail movement in sail
	control_map_[control_list[SAIL_UP]] = [this](float delta){
		move_sail(delta, -1);
	};
	
	control_map_[control_list[SAIL_DOWN]] = [this](float delta){
		move_sail(delta, 1);
	};

	//TODO controls for anchor, requires anchor refactor
	control_map_[control_list[ANCHOR_UP]] = [this](float delta){
		move_anchor(delta, -1);
	};

	control_map_[control_list[ANCHOR_DOWN]] = [this](float delta){
		move_anchor(delta, 1);
	};

}