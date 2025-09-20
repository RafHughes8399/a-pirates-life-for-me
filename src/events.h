#ifndef EVENTS_H
#define EVENTS_H
// std includes 
#include <functional>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <queue>
#include <ctime>
// project includes
#include "rendering.h"

namespace events{
	// an enum ID for event types
	enum event_types{
		test = 0,
		interaction = 1, // for example
		key_input = 2,
		camera_movement = 3,
		player_direction_change = 4,
		player_position_change = 5,
		anchor_change = 5,
		sail_length_change = 6,
		sail_wind_change = 7,
		size = 8 // update as needed
		/**
		 * types of events:
		 * 	-> collision
		 * -> player input
		 * -> camera movement (recalculating frustrum) ?
		 * -> items being picked up ?
		 */
	};
	class event{
	protected:
		bool handled_ = false;
		const int type_;
		float delay_; // potential execution delay for the event, i.e an event 
		// can take palce 10s after another,
	public:
		virtual ~event() = default;	
		event(int id, float delay=0.0f)
		: type_(id), handled_(false), delay_(delay){};
		
		event(event&& other) = default;
		event& operator=(event&& other) = default;
		
		bool is_handled(){
			return handled_;
		}
		const int get_type() const{
				return type_;
		}
		bool update_delay(float delta){
			delay_ = std::max(0.0f, delay_ - delta);
			return delay_ == 0;
		}
	};

	class test_event : public  event{
	protected:
	public:
		~test_event() = default;
		test_event(float delay=0.0f)
		: event(event_types::test, delay), time_(std::time(nullptr)){};

		static const int get_static_type(){
			return event_types::test;
		}
		char* get_event_time() const{
			return std::asctime(std::localtime(&time_));
		}
		private:
		std::time_t time_;

	};
	
	// define event subclasses
	//TODO add delay tp constructor
	/// @brief event to manage collisions between two entities
	class interaction_event :  public event{
	public:
		~interaction_event() = default;
		interaction_event() = default;

		static const int get_static_type(){
			return event_types::interaction;
		}
	private:
		
	};

	class player_input_event : public event{
	public:
		~player_input_event() = default;
		player_input_event(int key)
		: event(event_types::key_input), key_(key){};
		// somewhere there would need to be a key, function stored
		// like a player_controls class or something
		static const int get_static_type(){
			return event_types::key_input;
		}
		int get_key() const{
			return key_;
		}
	private:
		int key_;
		// something along the lines of:
	};
	
	// this event should provide the player with
	// the new position of the ship so that it can adjust the camera
	// position and target accordingly

	// the player would listen to such events
	// the ship moving would create such an event
	class camera_move_event : public event{
	public:
		~camera_move_event() = default;
		camera_move_event(Vector3& position_change)
		:event(event_types::camera_movement), position_change_(position_change){}
		
		Vector3 get_position() const{
			return position_change_;
		}
		static const int get_static_type(){
			return event_types::camera_movement;
		}
	private:
		Vector3& position_change_; // describes how the player ship has changed on each axis
	};

	// handler is templated for event types, there 
	// is a handler for each event type
	// TODO : 
	/**
	 * mini map change (direction and position)
	 * anchor hud change
	 * sail hud change (length and direction)
	 * 
	 * ! they take in the new value of the position or direction
	 * ! that is used directly to select the new frame of the animation of the hud component
	 */
	class player_direction_change_event : public event {
		public:
			~player_direction_change_event() = default;
			player_direction_change_event(int new_direction)
				: event(event_types::player_direction_change), new_direction_(new_direction) {};
			
			int get_new_direction() const{
				return new_direction_;
			}
			static const int get_static_type(){
				return event_types::player_direction_change;
			}
		private:
			// position change for the map and the direction for the compass
			int new_direction_; // selects the frame of the compass 
	};
	class player_position_change_event : public event {
		public:
			~player_position_change_event() = default;
			player_position_change_event(Vector2 new_position)
				: event(event_types::player_position_change),  new_position_(new_position) {};
			
			Vector2 get_new_position() const{
				return new_position_;
			}
			static const int get_static_type(){
				return event_types::player_position_change;
			}
		private:
			// position change for the map and the direction for the compass
			Vector2 new_position_; // selects the position of the ship on the map
	};
	class anchor_hud_change_event : public event{
		public:
			~anchor_hud_change_event() = default;
			anchor_hud_change_event(int new_depth)
				: event(event_types::anchor_change), new_depth_(new_depth){};

			int get_new_depth() const{
				return new_depth_;
			}
			static const int get_static_type(){
				return event_types::anchor_change;
			}
		private:
			int new_depth_; // the new frame for the anchor hud, based on the current depth / max depth

	};
	class sail_length_change_event : public event{
		public:
			~sail_length_change_event() = default;
			sail_length_change_event(int new_length, int new_force)
				: event(event_types::sail_length_change), new_length_(new_length){};
			
			int get_new_length() const{
				return new_length_;
			}
			static const int get_static_type(){
				return event_types::sail_length_change;
			}
		private:
			// length and direction in terms of wind force, in terms of the frame and aanimation for the hud
			int new_length_;
	};
	class sail_wind_change_event : public event{
		public:
			~sail_wind_change_event() = default;
			sail_wind_change_event( int new_force)
				: event(event_types::sail_length_change), new_force_(new_force){};
			
			int get_new_force() const{
				return new_force_;
			}
			static const int get_static_type(){
				return event_types::sail_length_change;
			}
		private:
			// length and direction in terms of wind force, in terms of the frame and aanimation for the hud
			int new_force_;
	};
	class event_handler_interface{
		public:
		virtual ~event_handler_interface() = default;
		void execute(const event& e){
			call_event(e);
		}
		virtual const int get_type() const = 0;
		private:
		virtual void call_event(const event& e) = 0;

	};
	
	template<typename E> // E for event
	class event_handler : public event_handler_interface{
	public:
		~event_handler() override = default;
		event_handler(std::function<void(const E& e)> handle)
			: handler_type_(E::get_static_type()), handler_(handle){};
		event_handler(const event_handler& other) = default;
		event_handler(event_handler&& other) = default;
		void call_event(const event& e) override{
			// check if event and handler template match, because you're doing a static 
			// cast
			if(e.get_type() == E::get_static_type()){
				// trigger the event stored in the handler
				handler_(static_cast<const E&>(e));
			}
			return;
		}
		const int get_type() const override{
			return handler_type_;
		}
		bool operator==(const event_handler& other){
			return handler_type_ == other.handler_type_; 
		}
	private:
		std::function<void(const E& e)> handler_;
		const int handler_type_;
	};

	
	// 
	// the event
	// the event handler - tempalted for any event, based on an interface
	// the dispatcher manages all the listeners for that 
	class event_dispatcher {
	public:
		void subscribe(int event_key, std::unique_ptr<event_handler_interface>& handler_value);
		void unsubscribe(int event_key, const int handler_value);
		void execute_event(const event& event);
		void queue_event(std::unique_ptr<event>& event);
		void add_delayed_event(std::unique_ptr<event>& event);
		void process_events(float delta);
	private:
		// for storing and processing events
		std::queue<std::unique_ptr<event>> event_queue_;
		std::vector<std::unique_ptr<event>> delayed_events_;
		// pairs an event id with instances of event handlers listening for the event
		std::unordered_map<int, std::vector<std::unique_ptr<event_handler_interface>>> subscriber_map_;
	};

	extern event_dispatcher global_dispatcher_;

}

#endif