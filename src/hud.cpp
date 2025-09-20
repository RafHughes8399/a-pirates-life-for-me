#include "hud.h"
// define the map
/* std::map<int, std::function<void(const events::event&)>> hud::hud_element_interface::on_event_map_ = {
    // TODO minimap and compass udpates for ship hud
    {events::event_types::map_change, [](const events::event& event) -> void{
        // cast the event
        const auto& map_event = static_cast<const events::map_change_event&>(event);
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



//TODO
void hud::player_change_strategy::on_event(const events::event& other){
    (void) other;
    // cast the event, then do what you need to do to the sprite 
    return; 
}