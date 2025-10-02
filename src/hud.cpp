#include "hud.h"


void hud::hud::hud_element::draw(){
    DrawTextureRec(hud_sprite_.get_sprite_sheet(), hud_sprite_.get_animation().get_frame(), position_, WHITE);
}
/*  void hud::hud::hud_element::on_event(const events::event& event){
	on_event_strategy_.on_event(event, hud_sprite_);
}
 */
void hud::hud::draw(){
    for(auto & element : elements_){
        element->draw();
    }
}
void hud::hud::clear(){
    elements_.clear();
}
size_t hud::hud::size(){
    return elements_.size();
}

void hud::hud::add_element(std::unique_ptr<hud_element>&& element){
    elements_.push_back(std::move(element)); // because copying is deleted 
}

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
    // build compass

    // build sail 

    // build anchor
    // ok so you build the element, which needs a sprite
    auto anchor_texture = LoadTexture(ANCHOR_HUD_PATH);
    auto anchor_sprite = sprite::sprite(anchor_texture, ANCHOR_HUD_WIDTH_FRAME, ANCHOR_HUD_HEIGHT_FRAME, ANCHOR_HUD_FRAMES, ANCHOR_HUD_ANIMATIONS);

    // a position 
    // for now the position will be rigid and based on the 1920 x 1080 resolution, 
    // TODO in the future is to make it scale based on the current machine resolution
    auto anchor_position = Vector2{50, 800}; // subject to change 

    // and a strategy
    std::unique_ptr<event_strategy> anchor_event_strategy = std::make_unique<anchor_height_change_strategy>(&anchor_sprite);

    // and then the hud elemement itself
    auto anchor_hud_element = std::make_unique<hud::hud_element>(anchor_sprite, anchor_position, std::move(anchor_event_strategy));
    // which is added to the hud
    hud_.add_element(std::move(anchor_hud_element));
    return;
}

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

void hud::anchor_height_change_strategy::on_event(const events::event& event, sprite::sprite& sprite){
    const events::anchor_hud_change_event& anchor_event = static_cast<const events::anchor_hud_change_event&>(event);
    // something along the lines of :

    auto depth = anchor_event.get_new_depth();
    // calculate proportion 
    float depth_proportion = depth / ANCHOR_MAX_DEPTH;
    int frame = sprite.get_animation().num_frames() * depth_proportion;
    sprite.get_animation().goto_frame(frame);
    // maybe some interemdiate maths to smooth the transition, that may be more to do with the actual changing part of the anchor's movement  ?
    return;
};


// need to cast the handler i think ? 

void hud::event_strategy::set_sprite_pointer(sprite::sprite* sprite_pointer){
    sprite_ = sprite_pointer;
}
void hud::anchor_height_change_strategy::subscribe(){
    auto* anchor_handler_cast = static_cast<events::event_handler<events::anchor_hud_change_event>*>(handler_.get());
    event_interface::subscribe<events::anchor_hud_change_event>(*anchor_handler_cast);
}
void hud::anchor_height_change_strategy::unsubscribe(){
    auto* anchor_handler_cast = static_cast<events::event_handler<events::anchor_hud_change_event>*>(handler_.get());
    event_interface::unsubscribe<events::anchor_hud_change_event>(*anchor_handler_cast);
}
/**
 * will uncomment as implemetned 
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
*/