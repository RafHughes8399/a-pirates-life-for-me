#include <functional>
#include <string>
#include <memory>
#include <vector>
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
		virtual ~event() = default;	
		event(int id)
		: type_(id), handled_(false){};

		bool handled_ = false;
		const int type_;

	public:
		bool is_handled(){
			return handled_;
		}
		const int get_type(){
			return type_;
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
		void execute(const event& e){
			call_event(e);
		}
		virtual std::string get_type() const = 0;
		private:
		virtual void call_event(const event& e) = 0;
	};
	
	template<typename E> // E for event
	class event_handler : event_handler_interface{
	public:
		event_handler() = default;
			/* TODO figure this out 		
			void call_event(const event& e) override{
			if(e.get_type() == E::get_static_event_type()){
				handler_(static_cast<const E&>(e));
			}
		} */
		const int get_type(){
			return handler_type_;
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
		void subscribe(const int& event_type, const std::unique_ptr<event_handler_interface>&& handler);
		void unsubscribe(const int& event_type, const int handler);
		void execute_event(const event& event);
		void queue_event(std::unique_ptr<event>&& event);
		void dispatch();
	private:
		// for storing and processing events
		std::vector<std::unique_ptr<event>> event_queue_;
		// pairs arn event id with instances of event handlers listening for the event
		std::unordered_map<int, std::vector<std::unique_ptr<event_handler_interface>>> subscriber_map_;
	};

	extern event_dispatcher dispatch_;

}