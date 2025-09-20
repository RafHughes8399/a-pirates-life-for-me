#include "hud.h"
// define the map
/* std::map<int, std::function<void(const events::event&)>> hud::hud_element_interface::on_event_map_ = {
    // TODO minimap and compass udpates for ship hud
    {events::event_types::player_direction_change, [](const events::event& event) -> void{
        // cast the event
        const auto& map_event = static_cast<const events::player_direction_change_event&>(event);
        // maybe this is where it happens instead of a completely different function , pass the sprite in too ?
        return;
    }},
    // TODO anchor updates for ship hud
    {events::event_types::anchor_change, [](const events::event& event) -> void{
        // cast the event

        // call the appropraite functions

    }},
    //TODO sail updates for ship hud
    {events::event_types::sail_change, [](const events::event& event)-> void{
        // cast the event

        // call the appropraite functions
    }}
};
 */


void hud::hud::draw(){
    for(auto & element : elements_){
        element->draw();
    }
}
void hud::hud::clear(){
    elements_.clear();
}

void hud::hud::add_element(std::unique_ptr<hud_element_interface> element){
    elements_.push_back(std::move(element));
}

// TODO implement
void hud::hud_builder::reset(){
    hud_.clear();
    return;
}

hud::hud& hud::hud_builder::get_hud(){
    return hud_;
}

//TODO implement
void hud::ship_hud_builder::build_map() {
    return;
}
//TODO implement
void hud::ship_hud_builder::build_player_components(){
    return;
}

//TODO implement
//TODO implement
void hud::pirate_hud_builder::build_map(){
    // create a hud element 

    return;
}
//TODO implement
void hud::pirate_hud_builder::build_player_components(){

    return;
}

hud::hud& hud::hud_director::build_hud(hud_builder& builder){
    builder.build_map();
    builder.build_player_components();
    return builder.get_hud();
}


void hud::player_direction_change_strategy::on_event(const events::event& event, sprite::sprite& sprite){
    // cast the event, then 
    const events::player_direction_change_event& map_event = static_cast<const events::player_direction_change_event&>(event);
    int direction_deg_frame = map_event.get_new_direction();

    // you need to do to the sprite, manipulate sprite with the info in the event 
    sprite.get_animation().goto_frame(direction_deg_frame);
}

//TODO implement the remainder of the strategies
void hud::player_position_change_strategy::on_event(const events::event& event, sprite::sprite& sprite){
    // cast the event 
    const events::player_position_change_event& pp_change_event = static_cast<const events::player_position_change_event&>(event);
    (void) sprite;
    // ? something along the lines of updating the frame of the animation
    // ? like the area that it covers 
    return;
};
void hud::anchor_height_change_strategy::on_event(const events::event& event, sprite::sprite& sprite){
    const events::anchor_hud_change_event& anchor_event = static_cast<const events::anchor_hud_change_event&>(event);
    // something along the lines of :
    auto frame = anchor_event.get_new_depth();
    sprite.get_animation().goto_frame(frame);
    // maybe some interemdiate maths to smooth the transition, that may be more to do with the actual changing part of the anchor's movement  ?
    return;
};
void hud::sail_length_change_strategy::on_event(const events::event& event, sprite::sprite& sprite){
    const events::sail_length_change_event& sail_length_event = static_cast<const events::sail_length_change_event&>(event);
    // something along the lines of 
    auto frame = sail_length_event.get_new_length();
    sprite.get_animation().goto_frame(frame);
    // similar to discussion in anchor
    return;
};
void hud::sail_wind_change_strategy::on_event(const events::event& event, sprite::sprite& sprite){
    const events::sail_wind_change_event& sail_wind_event = static_cast<const events::sail_wind_change_event&>(event);
    // something along the lines of 
    int anim = sail_wind_event.get_new_force();
    sprite.get_animation().goto_frame(anim);

    // similar to discussion in anchor
    return;
}