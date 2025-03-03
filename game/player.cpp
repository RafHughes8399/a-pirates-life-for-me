#include "player.h"

void Player::update(){
	
	// update the canera
	UpdateCamera(&camera_, CAMERA_THIRD_PERSON);
	ship_.set_position(camera_.target);
}
void Player::render() {
	// draw the ship
	ship_.render();
}