// includes, should make use of event should not be needed by entities
// std lib includes

// project includes
#include "events_interface.h"
/// @brief the essence of managers is to bridge the event system with 
// the other components of the game (like entities, game and world)

// a manager is how events are generated, so a collision maanger generates
// collision events
namespace managers{     
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