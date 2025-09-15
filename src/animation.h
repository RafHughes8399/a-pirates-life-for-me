
#ifndef ANIMATION_H
#define ANIMATION_H
#include "../lib/raylib/src/raylib.h"

// should listen for play events yk
/**
 * ? if you template it for an event you can choose the type of event the animation listens to ? 
 * ? the question is who listens and who is templated 
 * ? the sprite controls the animation so the sprite listens ? 
 * ? well actually it the hud element that would control the events so maybe 
 * ? the hud element should be
 * 
 * ? would probably need an animation builder / factory
 * 
 * 
 * the texture has the full width and length
 * if you pass in the sprite width and length and then the 
 * sheet width and length you can determine the number of animations and frames 
 * 
 */
namespace animation{
    class sprite_animation{
        public:
            ~sprite_animation() = default;
            sprite_animation(float frame_width, float frame_height, int frames, int animations)
            : frame_(Rectangle{0.0f, 0.0f, frame_width, frame_height}), frames_(frames), animations_(animations){

            }
            sprite_animation(const sprite_animation& other) = default;
            sprite_animation(sprite_animation&& other) = default;

            const Rectangle& get_frame() const;

            void next_frame(bool wrap = true);
            void next_animation();

            void goto_frame(const int frame);
            void goto_animation(const int animation);

            void play();
            void pause();
        private:
        // the frame of the sprite sheet
        Rectangle frame_;
        // num animations in the sheet (rows)
        int animations_; 
        // num frames in the animation (columns)
        int frames_;
        
        // default frame is the origin upon construction 
        int current_frame_ = 0;
        int current_animation_ = 0;
        bool is_playing = false;

    };
    // TODO: 3D model animation
    // class model_animation{}
} // namespace name

#endif