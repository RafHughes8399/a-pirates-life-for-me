#include "animation.h"


// TODO update the rectangle's position as well
const Rectangle& animation::sprite_animation::get_frame() const{
    return frame_;
}

void animation::sprite_animation::next_frame(bool wrap){
    // check bounds, increment, adjust rectangle, loop if wrap 
    if(current_frame_ < frames_ - 1){
        current_frame_++;
        frame_.x += frame_.width;
    }
    else if(wrap){
            current_frame_ = 0;
            frame_.x = 0.0;
    }
}
void animation::sprite_animation::next_animation(){
    // check bounds, increment, adjust rectangle
    // 0 to animations -1
    if(current_animation_ < animations_  - 1){
        current_animation_++;
    
        frame_.y += frame_.height;
    }
}

void animation::sprite_animation::goto_frame(const int frame){
    if(frame < frames_){
        current_frame_ = frame;
        frame_.x = frame_.width * frame;
    }
}

void animation::sprite_animation::goto_animation(const int animation){
    if(animation < animations_){
        current_animation_ = animation;
        frame_.y = frame_.height * animation;
    }
}

void animation::sprite_animation::play(){
    is_playing = true;
}
void animation::sprite_animation::pause(){
    is_playing = false;
}