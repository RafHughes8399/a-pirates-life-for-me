#ifndef HUD_H
#define HUD_H

#include "../lib/raylib/src/raylib.h"
#include "config.h"
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


/**
 * incorporating the handler into the pattern
 * event strategy abstract class has pointer to handler interface
 * concrete subclasses assign with pointer to the handler for the event type asscoaited 
 * with the strategy, using its override of the on_event function
 * 
 * overload unsub and sub that are called when the hud element is created and destroyed
 * 
 * i also have to solve the problem of hud copying, i think manageable 
 */
namespace hud   {

    class event_strategy{
        public:
            virtual ~event_strategy() = default;
			event_strategy(sprite::sprite* sprite)
				: sprite_(sprite){};
            virtual void on_event(const events::event& event, sprite::sprite& sprite) = 0;
			// ? i could template unsub and sub over an event type to avoid having to cast and repeat
			// ? essntially the same implementation in all my subclasses ?
			virtual void unsubscribe() = 0;
			virtual void subscribe() = 0;
			void set_sprite_pointer(sprite::sprite* sprite_pointer);
        protected:
			std::unique_ptr<events::event_handler_interface> handler_;
			sprite::sprite* sprite_; 
    };
	class anchor_height_change_strategy : public event_strategy{
		public:
		anchor_height_change_strategy(sprite::sprite* sprite)
		: event_strategy(sprite) {
			// create the handler
			handler_ = std::make_unique<events::event_handler<events::anchor_hud_change_event>>(
				[this](const events::anchor_hud_change_event& event) -> void{
					on_event(event, *sprite_);
				}
			);
		};
		void on_event(const events::event& event, sprite::sprite& sprite) override;
		void unsubscribe() override;
		void subscribe() override;

	};
	class player_direction_change_strategy : public event_strategy{
		public:
		player_direction_change_strategy(sprite::sprite* sprite)
		: event_strategy(sprite) {
			// and create the handler
			handler_ = std::make_unique<events::event_handler<events::player_direction_change_event>>(
				[this](const events::player_direction_change_event event) -> void {
					on_event(event, *sprite_);
				}
			);
		};
		
		void on_event(const events::event& event, sprite::sprite& sprite) override;
		void unsubscribe() override;
		void subscribe() override;
	};
	class sail_length_change_strategy : public event_strategy{
        public:
		sail_length_change_strategy(sprite::sprite* sprite)
        : event_strategy(sprite) {
			// create handler
			handler_ = std::make_unique<events::event_handler<events::sail_length_change_event>>(
				[this](const events::sail_length_change_event& event) -> void {
					on_event(event, *sprite_);
				}
			);
		};
        
        void on_event(const events::event& event, sprite::sprite& sprite) override;
		void unsubscribe() override;
		void subscribe() override;
	};
    
    class sail_wind_change_strategy : public event_strategy{
        public:
		sail_wind_change_strategy(sprite::sprite* sprite)
        : event_strategy(sprite) {
			handler_ = std::make_unique<events::event_handler<events::sail_length_change_event>>(
				[this](const events::sail_length_change_event& event) -> void {
					on_event(event, *sprite_);
				}
			);
		};
        
        void on_event(const events::event& event, sprite::sprite& sprite) override;
		void unsubscribe() override;
		void subscribe() override;
    };
    class player_position_change_strategy : public event_strategy{
        public:
		player_position_change_strategy(sprite::sprite* sprite)
        : event_strategy(sprite) {};
        
        void on_event(const events::event& event, sprite::sprite& sprite) override;
		void unsubscribe() override;
		void subscribe() override;
	};
	class hud{
		public:
			/**
			 * ? the eleement does not have an on_event, that is managed by the strategy
			 * ? the element manages the subscribing and unsubscribing of its strategy and passes
			 * ? it the necessary information to 
			 */
			class hud_element{
			public:
				~hud_element() {
					// unsub
					on_event_strategy_->unsubscribe();
				};
				// and an event strategy
				hud_element(sprite::sprite sprite, Vector2 position, std::unique_ptr<event_strategy>&& event_strategy)
				: hud_sprite_(sprite), position_(position), on_event_strategy_(std::move(event_strategy)){// construct it with the on_event method) {
					// sub
					on_event_strategy_->subscribe();
					on_event_strategy_->set_sprite_pointer(&hud_sprite_);
				}
				hud_element(const hud_element& other) = delete;
				hud_element(hud_element&& other) = default;

				hud_element& operator=(const hud_element& other) = delete;
				hud_element& operator=(hud_element&& other) = default;
				
				void draw();
				private:
				// sprite and an event handler, maybe make these part of the interfaso you can directly access them
				sprite::sprite hud_sprite_; // might transfer ownership to the strategy
				Vector2 position_;
				std::unique_ptr<event_strategy> on_event_strategy_; // this handles the on event 
			};
            ~hud() = default;
            hud() = default;

            hud(const hud& other) = delete;
            hud(hud&& other) = default;

			hud& operator=(const hud& other) = delete;
			hud& operator=(hud&& other) = default;
            void draw();
			void clear();
			void add_element(std::unique_ptr<hud_element>&& element);
			size_t size();
        private:
			std::vector<std::unique_ptr<hud_element>> elements_;
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