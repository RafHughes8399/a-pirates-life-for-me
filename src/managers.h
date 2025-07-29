#ifndef MANAGERS_H
#define MANAGERS_H
// includes, should make use of event should not be needed by entities
// std lib includes
#include <map>
#include <functional>

// project includes
#include "events_interface.h"
/// @brief the essence of managers is to bridge the event system with 
// the other components of the game (like entities, game and world)

// a manager is how events are generated, so a collision maanger generates
// collision events

// THIS IS A MEDIATOR PATTERN ? 
// i'm thinking, template the manager over an event type
// then somewhere map an event to an on_event function
// then in the general on_event defined in the class template, you call the function 
// stored at event 
// map event id to on_event thing ig
namespace managers{    
    // map events to their on_event function
    class event_map{
    public:
        // delete copy and move constructors
        event_map(const event_map& other) = delete;
        event_map(event_map&& other) = delete;

        event_map& operator=(const event_map& other) = delete;
        event_map& operator=(event_map&& other) = delete;

        static event_map& get_instance(){
            static event_map instance;
            return instance;
        }
        // accessing on_x_event functions 
        void on_event(const events::event& e);
        // on_x_event functions
        void on_test_event(const events::test_event& test);
    private:
        ~event_map() = default;
        event_map(){    
            on_event_map_[events::event_types::test] = [this](const events::event& e){
                on_test_event(static_cast<const events::test_event&>(e));
            };
        }
            std::map<int, std::function<void(const events::event& e)>> on_event_map_;
    };

    // so the event manager has the on event and the handler
    
    // then the constructor initialises the handler with the lambda callign on_event
    // and subscribes

    // the destructor unsubscribes
    template<typename E> // e for event
    class event_manager{
    public:
        ~event_manager(){
            // unsubscribe
            event_interface::unsubscribe<E>(handler_);
        }
        event_manager()
            : handler_([this](const E& event){ on_event(event);}){
                // subscribe
                event_interface::subscribe<E>(handler_);
            }
        void on_event(const E& event){
            event_map::get_instance().on_event(event);
        }
    private:
        events::event_handler<E> handler_;
    };

    
     class collision_manager{
        public:
        // TODO: in the destructor you unsub
        ~collision_manager() = default;
            // TODO in the constructor you sub
            collision_manager() = default;
            // TODO: the manager defines what happens on a collision event
            void on_collision_event(const events::collision_event& collision);
        private:
        // and a handler too
        events::event_handler<events::collision_event> collision_handler_;
    };
    
    // combat manager
    // quest mananger
    // player manager (for camera and key inputs)
    class player_manager{
        public:
        //TODO: unsub
        ~player_manager() = default;
        //TODO: subscribe
        player_manager() = default;
        
        //TODO on a camerea move event, recalculate the frustrum
        void on_camera_move_event(const events::camera_move_event& camera_move);
        // TODO on a key input event, call the function mapped to it
        void on_key_input_event(const events::key_input_event& key_input);
        
        private:
        // has the handler for both
        events::event_handler<events::camera_move_event> camera_move_handler_;
        events::event_handler<events::key_input_event> key_input_handler_;
    }; 
    // etc.
}
#endif