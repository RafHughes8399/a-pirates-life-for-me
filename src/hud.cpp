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
    // these are pending art implementation
    // build compass
    auto compass_texture = LoadTexture(COMPASS_HUD_PATH);
    auto compass_sprite = sprite::sprite(compass_texture, COMPASS_HUD_FRAME_WIDTH, COMPASS_HUD_FRAME_HEIGHT, COMPASS_HUD_FRAMES, COMPASS_HUD_ANIMATIONS);
    // ! placeholder, left of the anchor 
    auto compass_position = Vector2 {10, 800};
    //TODO 10/10 combine the two sail events into one 
    std::unique_ptr<event_strategy> compass_event_strategy = std::make_unique<player_direction_change_strategy>(&compass_sprite);
    auto sail_hud_element = std::make_unique<hud::hud_element>(compass_sprite, compass_position, std::move(compass_event_strategy));
    hud_.add_element(std::move(sail_hud_element));
    // build sail 
    auto sail_texture = LoadTexture(SAIL_HUD_PATH);
    auto sail_sprite = sprite::sprite(sail_texture, SAIL_HUD_FRAME_WIDTH, SAIL_HUD_FRAME_HEIGHT, SAIL_HUD_FRAMES, SAIL_HUD_ANIMATIONS);
    auto sail_position = Vector2 {10, 550};
    //TODO 10/10 combine the two sail events into one 
    std::unique_ptr<event_strategy> sail_event_strategy = std::make_unique<sail_length_change_strategy>(&sail_sprite);
    auto sail_hud_element = std::make_unique<hud::hud_element>(sail_sprite, sail_position, std::move(sail_event_strategy));
    hud_.add_element(std::move(sail_hud_element));

    // build minimap

    auto mini_map_texture = LoadTexture(MAP_HUD_PATH);
    auto mini_map_sprite = sprite::sprite(mini_map_texture, MAP_HUD_FRAME_WIDTH, MAP_HUD_FRAME_HEIGHT, MAP_HUD_FRAMES, MAP_HUD_ANIMATIONS);

    // ! placeholder, top right of the screen
    auto map_position = Vector2 {1800 ,100};
    std::unique_ptr<event_strategy> position_event_strategy = std::make_unique<player_position_change_strategy>(&mini_map_sprite);
    auto mini_map_hud_element = std::make_unique<hud::hud_element>(mini_map_sprite, map_position, std::move(position_event_strategy));
    hud_.add_element(std::move(mini_map_hud_element));
    // build anchor
    // ok so you build the element, which needs a sprite
    auto anchor_texture = LoadTexture(ANCHOR_HUD_PATH);
    auto anchor_sprite = sprite::sprite(anchor_texture, ANCHOR_HUD_WIDTH_FRAME, ANCHOR_HUD_HEIGHT_FRAME, ANCHOR_HUD_FRAMES, ANCHOR_HUD_ANIMATIONS);

    // a position 
    // for now the position will be rigid and based on the 1920 x 1080 resolution, 
    // TODO in the future is to make it scale based on the current machine resolution
    // ! placeholder - next to the compass, below the sail
    auto anchor_position = Vector2{200, 800}; // subject to change 

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

 void hud::player_direction_change_strategy::on_event(const events::event& event, sprite::sprite& sprite){
    // cast the event, then 
    const events::player_direction_change_event& map_event = static_cast<const events::player_direction_change_event&>(event);
    float direction_rad = map_event.get_new_direction();
    // calculaute the frame based on the degree, i think it is in radians, convert to deg
    float direction_degree = direction_rad * (180 / PI);

    // then pick the frame, should be the degree / 
    auto ratio = 360 / COMPASS_HUD_FRAMES;
    int frame = direction_degree / ratio;
    // you need to do to the sprite, manipulate sprite with the info in the event 
    sprite.get_animation().goto_frame(frame);}

void hud::sail_length_change_strategy::on_event(const events::event& event, sprite::sprite& sprite){
    const events::sail_length_change_event& sail_length_event = static_cast<const events::sail_length_change_event&>(event);
    float length = sail_length_event.get_new_length();
    // max length for the sail is 1 so length is length proportion
    int frame = sprite.get_animation().num_frames() * length;
    sprite.get_animation().goto_frame(frame);
    return;
};
void hud::sail_wind_change_strategy::on_event(const events::event& event, sprite::sprite& sprite){
    const events::sail_wind_change_event& sail_wind_event = static_cast<const events::sail_wind_change_event&>(event);
    // different as it changes the animation instead of the frame 
    int force_proportion = sail_wind_event.get_new_force();
    int animation = sprite.get_animation().num_animations() * force_proportion;
    sprite.get_animation().goto_animation(animation); 
    return;
}
void hud::player_position_change_strategy::on_event(const events::event& event, sprite::sprite& sprite){
    // cast the event 
    const events::player_position_change_event& pp_change_event = static_cast<const events::player_position_change_event&>(event);
    // new position of the ship
    auto new_ship_position = pp_change_event.get_new_position(); 
    // the new position should be the centre of the rectangle, 
    // the rectangle's position is based on the top left corner of the rectangle, 
    // the ship should be at the centre of the minimap so
    // calculate the centre of the frame
    auto current_frame = sprite.get_animation().get_frame();
    Vector2 frame_centre = Vector2{current_frame.x + (current_frame.width / 2), current_frame.y + (current_frame.height / 2)};
    Vector2 differnce = Vector2{new_ship_position.x - frame_centre.x, new_ship_position.y - frame_centre.y};
    // the difference between the current centre and the new centre is how much you change the new position of the frame by
    Vector2 new_frame_position = Vector2{current_frame.x + differnce.x, current_frame.y + differnce.y};
    sprite.get_animation().set_frame_position(new_frame_position);
    return;
};
void hud::event_strategy::set_sprite_pointer(sprite::sprite* sprite_pointer){
    sprite_ = sprite_pointer;
}
void hud::anchor_height_change_strategy::subscribe(){
    auto* casted_handler = static_cast<events::event_handler<events::anchor_hud_change_event>*>(handler_.get());
    event_interface::subscribe<events::anchor_hud_change_event>(*casted_handler);
}
void hud::anchor_height_change_strategy::unsubscribe(){
    auto* casted_handler = static_cast<events::event_handler<events::anchor_hud_change_event>*>(handler_.get());
    event_interface::unsubscribe<events::anchor_hud_change_event>(*casted_handler);
}

void hud::player_direction_change_strategy::subscribe(){
    auto* casted_handler = static_cast<events::event_handler<events::player_direction_change_event>*>(handler_.get());
    event_interface::subscribe<events::player_direction_change_event>(*casted_handler);
}
void hud::player_direction_change_strategy::unsubscribe(){
    auto* casted_handler = static_cast<events::event_handler<events::player_direction_change_event>*>(handler_.get());
    event_interface::unsubscribe<events::player_direction_change_event>(*casted_handler);
}

void hud::sail_length_change_strategy::subscribe(){
    auto* casted_handler = static_cast<events::event_handler<events::sail_length_change_event>*>(handler_.get());
    event_interface::subscribe<events::sail_length_change_event>(*casted_handler);
}
void hud::sail_length_change_strategy::unsubscribe(){
    auto* casted_handler = static_cast<events::event_handler<events::sail_length_change_event>*>(handler_.get());
    event_interface::unsubscribe<events::sail_length_change_event>(*casted_handler);
}

void hud::sail_wind_change_strategy::subscribe(){
    auto* casted_handler = static_cast<events::event_handler<events::sail_wind_change_event>*>(handler_.get());
    event_interface::subscribe<events::sail_wind_change_event>(*casted_handler);
}
void hud::sail_wind_change_strategy::unsubscribe(){
    auto* casted_handler = static_cast<events::event_handler<events::sail_wind_change_event>*>(handler_.get());
    event_interface::unsubscribe<events::sail_wind_change_event>(*casted_handler);
}
void hud::player_position_change_strategy::subscribe(){
    auto* casted_handler = static_cast<events::event_handler<events::sail_wind_change_event>*>(handler_.get());
    event_interface::subscribe<events::sail_wind_change_event>(*casted_handler);
}
void hud::player_position_change_strategy::unsubscribe(){
    auto* casted_handler = static_cast<events::event_handler<events::player_position_change_event>*>(handler_.get());
    event_interface::unsubscribe<events::player_position_change_event>(*casted_handler);
}

