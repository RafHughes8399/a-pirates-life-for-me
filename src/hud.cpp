#include "hud.h"

void hud::hud::draw(){
    state_->draw();
}

void hud::hud_state::draw(){
    for(auto & element : elements_){
        element.draw();
    }
}