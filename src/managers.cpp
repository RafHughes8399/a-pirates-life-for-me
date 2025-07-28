#include "managers.h"

//TODO implement
void managers::collision_manager::on_collision_event(const events::collision_event& collision){
    (void) collision;
    // ok so take  the two entities in the collision event and 
    // process them here

    return;
}
// i think these two will be more simple
//TODO implement
void managers::player_manager::on_camera_move_event(const events::camera_move_event& camera_movement){
    (void) camera_movement;
    // ok take the camera position
    // update the position and recalc the frustrum 
    // i'm going to need the player reference 
    return; 
}
// TODO implement
void managers::player_manager::on_key_input_event(const events::key_input_event& key_input){
    (void) key_input;
    // take the key input in the event
    // and the control map, then call the function 
    // ok the player manager is going to need the player
    return;
}