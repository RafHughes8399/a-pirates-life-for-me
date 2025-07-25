#include <functional>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <queue>
namespace events{
	// an enum ID for event types
	enum event_types{
		test = 0,
		collision = 1 // for example
		/**
		 * types of events:
		 * 	-> collision
		 * -> player input
		 * -> camera movement (recalculating frustrum)
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
		event(int id)
		: type_(id), handled_(false){};
		
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

	class test_event : event{
	protected:
	public:
		test_event()
		: event(event_types::test){};

		static const int get_static_event_type(){
			event_types::test;
		}

	};
	// define event subclasses



	// handler is templated for event types, there 
	// is a handler for each event type
	
	class event_handler_interface{
		public:
		virtual ~event_handler_interface() = default;
		void execute(const event& e){
			call_event(e);
		}
		virtual const int get_type() = 0;
		private:
		virtual void call_event(const event& e) = 0;

	};
	
	template<typename E> // E for event
	class event_handler : event_handler_interface{
	public:
		~event_handler() override = default;
		event_handler() = default;
		void call_event(const event& e) override{
			//TODO: implement
			return;
		}
		const int get_type() override{
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
		void dispatch(float delta);
	private:
		// for storing and processing events
		std::queue<std::unique_ptr<event>> event_queue_;
		std::vector<std::unique_ptr<event>> delayed_events_;
		// pairs arn event id with instances of event handlers listening for the event
		std::unordered_map<int, std::vector<std::unique_ptr<event_handler_interface>>> subscriber_map_;
	};

	extern event_dispatcher dispatch_;

}