#include "player.h"

void player::hud::draw(){
    for(auto & element : elements_){
        element.draw();
    }
}