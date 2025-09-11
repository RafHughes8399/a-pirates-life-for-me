#include "entities.h"
#include "config.h"
int entities::player_ship::update(float delta){
	//MoveableObject::update(delta);
	// update the anchor
	// apply gravity
	acceleration_.y += GRAVITY;

	// this calculates the veloctuty for a given frame
	// apply the sail movement coeffieicts
	auto sail_force = sail_.get_force();
	acceleration_ = Vector3Add(acceleration_, sail_.get_force());
	
	// apply the anchor force coefficient 
	auto anchor_force = anchor_.get_force();
	acceleration_ = Vector3Multiply(acceleration_, anchor_.get_force());
	
	std::cout << "acceleration: " << acceleration_.y << std::endl;
	// apply acceleration to velocity
	velocity_ = acceleration_;

	// apply the player_ship direction to the velocity, use sin and cos, other way around z is cos, sin is x
	auto direction_coefficient = get_direction_coefficient();
	velocity_ = Vector3Multiply(velocity_, direction_coefficient);
	
	// update pos
	velocity_ = Vector3Scale(velocity_, delta);
	position_ = Vector3Add(position_, velocity_);
	
	// create an event 
	std::unique_ptr<events::event> ship_moved_event = std::make_unique<events::camera_move_event>(velocity_);
	event_interface::queue_event(ship_moved_event);
	
	// reset accel 
	acceleration_ = Vector3Zero();

	// update the bounding box 
	bounding_box_.min = Vector3Add(bounding_box_.min, velocity_);
	bounding_box_.max = Vector3Add(bounding_box_.max, velocity_);

	if(not Vector3Equals(velocity_, Vector3Zeros)){
		return entities::status_codes::moved;
	}
	else{
		return entities::status_codes::nothing;
	}
}
void entities::player_ship::render(){
	DrawModel(object_type_.get_model(), position_, 0.15f, WHITE);
	DrawBoundingBox(bounding_box_, RED);
}

void entities::player_ship::interact(entities::entity& other){
	auto static_entity_ptr = dynamic_cast<entities::static_entity*>(&other);
	if(static_entity_ptr){
		// consider terrain
	}
	else{
		auto moveable_entity_ptr = dynamic_cast<entities::moveable_entity*>(&other);
		
	}
	// static objects include terrain

	// moveable objects include other ships 
	return;
}
std::unique_ptr<entities::entity> entities::player_ship::clone(){
	return nullptr;
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
	auto delta = GetFrameTime();
	auto ship_control = control_map_[event.get_key()];
	ship_control(delta);
}
void entities::player_ship::steer_ship(float delta, int direction){
	auto turn = SHIP_TURN_SPEED * delta * direction;
	// TODO make sure this doesn't become negative
	auto new_direction = std::fmod(direction_ + turn, PI2);
	
	// if the value were to become negative, instead add 2PI?, yes shorty
	direction_ = new_direction < 0 ? new_direction + PI2 : new_direction;

	turn_sail(delta, direction);
	auto rotate_vector = Vector3 {0.0f, direction_, 0.0f};
	object_type_.get_model().transform = MatrixRotateXYZ(rotate_vector);
}

void entities::player_ship::move_sail(float delta, int direction){
	float move = LOWER_RAISE_SPEED * delta;
	sail_.move(move, direction);
}

void entities::player_ship::turn_sail(float delta, int turn_direction){
	sail_.turn(delta, direction_, turn_direction);
	return;
}
void entities::player_ship::move_anchor(float delta, int direction){
	auto move = delta * ANCHOR_MOVE_SPEED;
	anchor_.move(delta, direction);
	return;
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

	// TODO refactor sail turning
	control_map_[control_list[SAIL_LEFT]] = [this](float delta){
		turn_sail(delta, 1);
	};
	control_map_[control_list[SAIL_RIGHT]] = [this](float delta){
		turn_sail(delta, -1);
	};
	//TODO controls for anchor, requires anchor refactor
	control_map_[control_list[ANCHOR_UP]] = [this](float delta){
		move_anchor(delta, -1);
	};

	control_map_[control_list[ANCHOR_DOWN]] = [this](float delta){
		move_anchor(delta, 1);
	};

}