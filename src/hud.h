#ifndef HUD_H
#define HUD_H

#include "../lib/raylib/src/raylib.h"
#include "sprite.h"
#include "events.h"
#include <vector>
#include <memory>
// the hud has an abstract hud_strategy, which is a concrete strategy, just swap the strategy
// or you just build two instances of the hud

// you listen to direction changes, anchor changes, sail changes and positional changes

// i think a hud strategy is necessary, because they behave somewhat differently right
// in terms of the events they listen to, the elements they draw

// well the behaviour doesn't really change, just the elements 

// maybe a hud builder ?  it is more the contents that differ, less so the behaviour 
// and then upon events happening the hud would change ? or rather be rebuilt ?

// ? consider the way you will implement hud switching
// ? possibilites include a hud state or a hud strategy that change in response to events triggered by the player ? 
// ? the state / strategy define the specific elements of the hud
// ? i think state perhaps, because the behaviour is the same, all that differs is the content
namespace hud   {
    class hud{
        public:
            ~hud() = default;
            hud() = default;
            hud(const hud& other) = default;
            hud(hud&& other) = default;

            void draw();

        private:
            std::unique_ptr<hud_state> state_;
    };
    class hud_state{
        public:
            virtual ~hud_state() = default;
            hud_state()  = default; // empty hud 
            
            template<typename InputIt>
            hud_state(InputIt begin, InputIt end)
            : elements_(begin, end){};

            hud_state(const hud_state& other) = default;
            hud_state(hud_state&& other) = default;

            void draw();
            virtual void build_hud() = 0;

        protected:
            std::vector<hud_element<events::event>> elements_;
    };

    class pirate_hud_state : public hud_state{
        public:
            void build_hud() override;
        private:
    };
    class ship_hud_state : public hud_state{
        public:
            void build_hud() override;
        private:
    };
    /**
     * hud elements are templated for an event type so the event that they listen to can be specified easily without creating 
     * heaps of near_identical subclasses
     * its so they can handle changes that occur throughout the game according to player input 
     * TODO consider how to incorporate event handling into the hud element 
     */
    template <typename E> // E for event
    class hud_element{
        public:
            ~hud_element() = default;
            hud_element() = default;
            hud_element(const hud_element& other) = default;
            hud_element(hud_element&& other) = default;

            void draw() const{
                DrawTextureRec(hud_sprite_.get_sprite_sheet(), hud_sprite_.get_animation().get_frame(), position_, WHITE);
            }
            // on event stuff as well, not sure exactly how this will work just yet though, 
            /**
             // TODO look into the template lecture slides to figure this one out, i think i can define things like on_event<Anchor_hud_event_update> () 
             * the on event method is tmeplated for E and then you define the different possiblities in the cpp file ? 
             for now just sub and unsub 
             * 
             */
        private:
            // sprite and an event handler
            sprite::sprite hud_sprite_;
            Vector2 position_;
            events::event_handler<E> handler_;
    };
} // namespace name

#endif