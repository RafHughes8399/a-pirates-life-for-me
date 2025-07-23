#include "events.h"
//TODO: implement
void events::event_dispatcher::subscribe(const int& event_type, const std::unique_ptr<event_handler_interface>&& handler){
    (void) event_type;
    (void) handler;
    return;
}
void events::event_dispatcher::unsubscribe(const int& event_type, const int handler){
    (void) event_type;
    (void) handler;
    return;
}
void events::event_dispatcher::execute_event(const event& event){
    (void) event;
}
void events::event_dispatcher::queue_event(std::unique_ptr<event>&& event){
    (void) event;
}
void events::event_dispatcher::dispatch(){
    return;
}
