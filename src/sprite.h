#ifndef SPRITE_H
#define SPRITE_H
#include "animation.h"
namespace sprite{
    /** 
     * ? maybe a sprite factor could help to build certain sprites, look into a factory pattern
     * */
    class sprite{
        public:
            ~sprite() = default;
            sprite(Texture2D& sprite_sheet, float sprite_width, float sprite_height)
            :   sprite_sheet_(sprite_sheet), animation_(animation::sprite_animation(sprite_width, sprite_height, 
                sprite_sheet.width / sprite_width, sprite_sheet.height / sprite_height)){

            };
            sprite(const sprite& other) = default;
            sprite(sprite&& other) = default;

            animation::sprite_animation& get_animation();
            Texture2D& get_sprite_sheet();

        private:
            Texture2D sprite_sheet_;
            animation::sprite_animation animation_;
    };
} // namespace sprite

#endif
