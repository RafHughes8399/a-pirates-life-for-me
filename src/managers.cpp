#include "managers.h"


void managers::event_map::on_event(const events::event& e){
    // call the on event function for the event id provided
    on_event_list_[e.get_type()](e);
    return;

}

void managers::event_map::on_test_event(const events::test_event& test){
    std::cout << test.get_event_time() << std::endl;
    // TODO something
}
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