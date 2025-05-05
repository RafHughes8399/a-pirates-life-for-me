
#include "collision_events.h"
#include <memory>

class EventHandler {
public:
	EventHandler()
		: collision_event_dispatcher_() {
	}
	static EventHandler* get_instance();
	EventDispatcher<CollisionEvents> collision_event_dispatcher_;
private:

	static std::unique_ptr<EventHandler> instance_;
};

#define ADD_COLLISION_EVENT_LISTENER(eventType, func, arg) EventHandler::get_instance()->collision_event_dispatcher_.add_listener(eventType, std::bind(&func, arg, std::placeholders::_1));
#define REMOVE_COLLISION_EVENT_LISTENER(handle) EventHandler::get_instance()->collision_event_dispatcher_.remove_listener(handle);

#define SEND_COLLISION_EVENT(_event) EventHandler::get_instance()->collision_event_dispatcher_.send_event(_event);