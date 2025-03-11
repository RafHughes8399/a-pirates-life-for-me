#include "../objects/ship_components.h"
Vector3 Sail::get_sail_direction(){
	return direction_;
}

float Sail::get_sail_length(){
	return length_;
}

void Sail::move_sail(Vector3 offset){
	direction_ = Vector3Add(direction_, offset);
}

void Sail::raise_sail(float length) {
	// 0 <=
	length_ = std::max(0.0f, (length_ - length));
}

void Sail::drop_sail(float length){
	// <= 1
	length_ += std::min(1.0f, (length_ + length));

}

void Anchor::lower(){
	state_->lower(this);
}

void Anchor::raise(){
	state_->raise(this);
}

void Anchor::update(){
	// do the same min max thing 
	auto speed = state_->get_speed();
	// so the ship is raising
	if(speed < 0.0){
		depth_ = std::max(0.0f, depth_ - speed);
		if (depth_ == 0.0) {
			state_.reset(new RaisedState(ANCHOR_STATIC_SPEED, Vector3{ 1.0f, 0.0f, 1.0f }));
		}	
	}
	else {
		depth_ = std::min(ANCHOR_MAX_DEPTH, depth_ + speed);
		if (depth_ == ANCHOR_MAX_DEPTH) {
			state_.reset(new LoweredState(ANCHOR_STATIC_SPEED, Vector3{ 0.0f, 0.0f, 0.0f }));
		}
	}
	depth_ += state_->get_speed();
}

Vector3 Anchor::get_force(){
	return state_->get_force(); // incorporate the height somehow
}


// it is about changing the state, the state has the speed

void Anchor::RaisedState::lower(Anchor* anchor){
	// change the state to lowering
	anchor->state_.reset(new LoweringState(ANCHOR_DROP_SPEED, Vector3 {0.8f,0.0f, 0.8f}));
}

void Anchor::RaisedState::raise(Anchor* anchor){
	// do nothing
	return;
}


void Anchor::LoweredState::lower(Anchor* anchor) {
	// do nothing
	return;
}

void Anchor::LoweredState::raise(Anchor* anchor){
	// change state to raising
	anchor->state_.reset(new RaisingState(ANCHOR_DROP_SPEED * -1, Vector3 {0.8f,0.0f, 0.8f}));
}

void Anchor::LoweringState::lower(Anchor* anchor) {
	// do nothing;
	return;
}


void Anchor::LoweringState::raise(Anchor* anchor){
	// change state to raising
	anchor->state_.reset(new RaisingState(ANCHOR_DROP_SPEED * -1, Vector3{ 0.8f,0.0f, 0.8f }));
	return;
}


void Anchor::RaisingState::lower(Anchor* anchor){
	// change the state to lowering
	anchor->state_.reset(new LoweringState(ANCHOR_DROP_SPEED, Vector3{ 0.8f,0.0f, 0.8f }));
	return;
}


void Anchor::RaisingState::raise(Anchor* anchor){
	// do nothing 
	return;
}

float Anchor::AnchorState::get_speed()
{
	return speed_;
}

Vector3 Anchor::AnchorState::get_force(){
	return force_coefficient_;
}
