#include "sprite.h"
animation::sprite_animation& sprite::sprite::get_animation(){
    return animation_;
}
Texture2D& sprite::sprite::get_sprite_sheet(){
    return sprite_sheet_;
}