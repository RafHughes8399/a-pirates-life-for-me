#include "event_handler.h"


std::unique_ptr<EventHandler> EventHandler::instance_ = nullptr;
EventHandler* EventHandler::get_instance()
{
	if (not instance_) {
		instance_ = std::make_unique<EventHandler>();
	}
	return instance_.get();
}
