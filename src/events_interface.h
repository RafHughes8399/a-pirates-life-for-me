#include "events.h"

// clean method interface for events to be used throughout the project
//TODO: pending implementation
template<typename E>
inline void subscribe(const events::event_handler<E>& handler){
    (void) handler;
} 

template<typename E>
inline void unsubscribe(const events::event_handler<E>& handler){
    (void) handler;
} 


inline void execute_event(const events::event& event){
    (void) event;
}

inline void queue_event(std::unique_ptr<events::event>&& event){
    (void) event;
}