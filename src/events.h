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
//#include "entities.h" // will need to change the build order ? 
//#include "player.h"
#include "rendering.h"
// other includes

namespace events{
	// an enum ID for event types
	enum event_types{
		test = 0,
		collision = 1, // for example
		key_input = 2,
		camera_movement = 3,
		num_types = 4 // update as needed
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
	class collision_event :  public event{
	public:
		~collision_event() = default;
		collision_event() = default;

		static const int get_static_type(){
			return event_types::collision;
		}
	private:
		//entities::entity& a_;
		//entities::entity& b_;

		// a collision event takes two entities and processes 
		// what happens when those two entities collide

		// entities needs events 
		// events need entities
		
		// unless, the world handles collisions
		// or there is a separate collision manager
		// use an intermediate class between event and entities to 
		// manage the collision, it is abridge between the two
		// then the game or the world has the collision manager
		// the game would have the system, access the entities through the world


		// there's a design pattern for that right, let's have a look
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
	
	// mainly used to recalcualte the frustrum when the camera moves
	class camera_move_event : public event{
	public:
		~camera_move_event() = default;
		camera_move_event(Vector3& position, Camera3D& camera, rendering::frustrum& frustrum)
		:event(event_types::camera_movement), new_position_(position), camera_(camera),
		frustrum_(frustrum){};

		static const int get_static_type(){
			return event_types::camera_movement;
		}
	private:
		Vector3& new_position_;
		Camera3D& camera_;
		rendering::frustrum& frustrum_;
	};

	// handler is templated for event types, there 
	// is a handler for each event type
	
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