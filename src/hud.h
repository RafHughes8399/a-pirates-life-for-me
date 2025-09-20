#ifndef HUD_H
#define HUD_H

#include "../lib/raylib/src/raylib.h"
#include "sprite.h"
#include "events.h"
#include "events_interface.h"
#include <vector>
#include <memory>
#include <map>
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

    class event_strategy{
        public:
            virtual ~event_strategy() = default;
            event_strategy() = default;
            event_strategy(const event_strategy& other) = default;
            event_strategy(event_strategy&& other) = default;

            event_strategy& operator=(const event_strategy& other) = default;
            event_strategy& operator=(event_strategy&& other) = default;
            
            virtual void on_event(const events::event& event) = 0;
        protected:

    };
    class player_change_strategy : public event_strategy{
        public:
            player_change_strategy()
            : event_strategy() {};

            void on_event(const events::event& event) override;
        private:
    };

	class hud_element_interface{
		public:
			virtual ~hud_element_interface() = default;
			virtual void draw() = 0;
			static std::map<int, std::function<void(const events::event&)>> on_event_map_;
			// TODO define on event functions
			void on_anchor_move_event(const events::anchor_hud_change_event& event);
			void on_player_move_event(const events::map_change_event& event);
			void on_player_turn_event(const events::map_change_event& event);
			void on_sail_move_event(const events::sail_hud_change_event& event);
			void on_sail_turn_event(const events::sail_hud_change_event event);

			static void register_handler(int event_type, std::function<void(const events::event&)> handler){
				on_event_map_[event_type] = handler;
			}
	};
	template <typename E> // E for event
	class hud_element : public hud_element_interface{
		public:
			~hud_element() {
				// unsub
				event_interface::unsubscribe<E>(handler_);
			};
			hud_element(sprite::sprite& sprite, Vector2 position)
			: hud_sprite_(sprite), position_(position), handler_([this](const E& event) -> void {on_event(event);}){// construct it with the on_event method) {
				// sub
				event_interface::subscribe<E>(handler_);
			}
			hud_element(const hud_element<E>& other) = default;
			hud_element(hud_element<E>&& other) = default;

			void draw(){
				DrawTextureRec(hud_sprite_.get_sprite_sheet(), hud_sprite_.get_animation().get_frame(), position_, WHITE);
			}
			void on_event(const E& event){
				// examine the map defined by the interface, pass in the data 
				// access the map by using the id of E, to access the specific on event fucntion
				// and pass in the appropraite sprite data
				// ? can it ?, well yes it can
                //?  perhaps requires a reference to the self to be passed through ? 
				hud_element_interface::on_event_map_[E::get_static_type()](event); // ? maybe the sprite too ? 
			}
			private:
			// sprite and an event handler, maybe make these part of the interfaso you can directly access them
			sprite::sprite hud_sprite_;
			Vector2 position_;
			events::event_handler<E> handler_;
            event_strategy& on_event_strategy_;
	};
	class hud{
		public:
            ~hud() = default;
            hud() = default;

			// TODO implement with deep copy 
            hud(const hud& other)
			: elements_() {
				// deep copy the elements
				for(auto & elem : other.elements_){
					// ? assuming each hud_element has a clone method
					// ? elements_.push_back(elem->clone());
				}
			}
            hud(hud&& other) = default;

			hud& operator=(const hud& other){
				if(this != &other){
					elements_.clear();
					for(auto & elem : other.elements_){
						// ? assuming each hud_element has a clone method
						// ? elements_.push_back(elem->clone());
					}
				}
				return *this;
			}
			hud& operator=(hud&& other) = default;
            void draw();
			void clear();
			void add_element(std::unique_ptr<hud_element_interface> element);
        private:
			std::vector<std::unique_ptr<hud_element_interface>> elements_;
	};
	// TODO implement overrides, pending art 
	class hud_builder {
		public:
			virtual ~hud_builder() = default;
			hud_builder()
				: hud_(hud()){};
	
			void reset();
			hud& get_hud();

			// map components
			virtual void build_map() = 0;
			//virtual hud build_quests() = 0;
			virtual void build_player_components() = 0;
		protected:
			hud hud_;
	};
	class ship_hud_builder : public hud_builder{
		public:
		ship_hud_builder() 
			:hud_builder(){};
		
		void build_map() override;
		void build_player_components() override;
	};
	class pirate_hud_builder : public hud_builder{
		public:
		pirate_hud_builder() 
			:hud_builder(){};
		
		void build_map() override;
		void build_player_components() override;
	};
	class hud_director{
		// static build ship hud, static build pirate hud
		public:
		static hud& build_hud(hud_builder& builder);
	};

} // namespace name
#endif