#include "player.h"
// define the map
std::map<int, std::function<void(const events::event&)>> player::hud::hud_element_interface::on_event_map_ = {};// TODO and fill it with the evenets


void player::hud::draw(){
    for(auto & element : elements_){
        element->draw();
    }
}

void player::hud::clear(){
    elements_.clear();
}

void player::hud::add_element(std::unique_ptr<hud_element_interface> element){
    elements_.push_back(std::move(element));
}

// TODO implement
void player::hud_builder::reset(){
    hud_.clear();
    return;
}

player::hud& player::hud_builder::get_hud(){
    return hud_;
}

//TODO implement
void player::ship_hud_builder::build_map() {
    return;
}
//TODO implement
void player::ship_hud_builder::build_player_components(){
    return;
}

//TODO implement
//TODO implement
void player::pirate_hud_builder::build_map(){
    // create a hud element 

    return;
}
//TODO implement
void player::pirate_hud_builder::build_player_components(){

    return;
}

player::hud& player::hud_director::build_hud(hud_builder& builder){
    builder.build_map();
    builder.build_player_components();
    return builder.get_hud();
}