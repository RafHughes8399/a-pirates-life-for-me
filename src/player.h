#ifndef PLAYER_H
#define PLAYER_H
#include <map>
#include <functional>
#include <utility>

#include "entities.h"
#include "config.h"
#include "rendering.h"
#include "events_interface.h"
#include "controls.h"
#include "sprite.h"

#include "../lib/raylib/src/raylib.h"
#include "../lib/raylib/src/raymath.h"
#include "../lib/raylib/src/rcamera.h"


// ok I need to define a constant distance betwen the camera position and the 
// target, as the target (the ship) moves, so too must the camera. it must move the same amount 

namespace player{
	enum huds{
		ship = 0,
		pirate = 1,
		// include more if there are
		size = 2
	};
	class hud{
		/**
		 * hud elements are templated for an event type so the event that they listen to can be specified easily without creating 
		 * heaps of near_identical subclasses
		 * its so they can handle changes that occur throughout the game according to player input 
		 * TODO consider how to incorporate event handling into the hud element 
		 */
		template <typename E> // E for event
		class hud_element{
			public:
				~hud_element() = default;
				hud_element() = default;
				hud_element(const hud_element& other) = default;
				hud_element(hud_element&& other) = default;

				void draw() const{
					DrawTextureRec(hud_sprite_.get_sprite_sheet(), hud_sprite_.get_animation().get_frame(), position_, WHITE);
				}
				// on event stuff as well, not sure exactly how this will work just yet though, 
				/**
				 // TODO look into the template lecture slides to figure this one out, i think i can define things like on_event<Anchor_hud_event_update> () 
				* the on event method is tmeplated for E and then you define the different possiblities in the cpp file ? 
				for now just sub and unsub 
				* 
				*/
			private:
				// sprite and an event handler
				sprite::sprite hud_sprite_;
				Vector2 position_;
				events::event_handler<E> handler_;
		};
        public:
            ~hud() = default;
            hud() = default;
            hud(const hud& other) = default;
            hud(hud&& other) = default;

			hud& operator=(const hud& other) = default;
			hud& operator=(hud&& other) = default;
            void draw();

        private:
			std::vector<hud_element<events::event>> elements_;
	};
	class hud_builder {
		public:
			virtual ~hud_builder() = default;
			hud_builder()
				: hud_(hud()){};
	
			void reset();
			hud get_hud();

			// map components
			virtual void build_map() = 0;
			//virtual hud build_quests() = 0;
			virtual void build_player_components() = 0;
		protected:
			hud hud_;
	};
	class ship_hud_builder : public hud_builder{
		public:
		ship_hud_builder() 
			:hud_builder(){};
		
		void build_map() override;
		void build_player_components() override;
	};
	class pirate_hud_builder : public hud_builder{
		public:
		pirate_hud_builder() 
			:hud_builder(){};
		
		void build_map() override;
		void build_player_components() override;
	};
	class hud_director{
		// static build ship hud, static build pirate hud
		public:
		static hud build_ship_hud(hud_builder& builder);
		static hud build_pirate_hud(hud_builder& builder);
	};
	class player {
		public:
			~player(){
				// unsubscribe
				event_interface::unsubscribe<events::camera_move_event>(camera_movement_handler_);
			}
			player()
			:camera_(Camera3D{}), camera_mode_(CAMERA_THIRD_PERSON),
			camera_frustrum_(camera_, ASPECT_RATIO, FOV, NEAR, FAR), 
			camera_target_distance_(Vector3Subtract(SHIP_START, CAMERA_START)),
			camera_movement_handler_([this](const events::camera_move_event& event){ on_camera_move_event(event);}){
				camera_.position = CAMERA_START;
				camera_.target = SHIP_START;// the camera looks at the cube, slightly above sea level
				camera_.up = Vector3{ 0.0, 1.0, 0.0 }; // rotation toward target
				camera_.fovy = FOV;
				camera_.projection = CAMERA_PERSPECTIVE; // should be third person mode ?
				

				// something along the lines of 
				build_huds();
				// subscribe
				event_interface::subscribe<events::camera_move_event>(camera_movement_handler_);
			}
			player(const player& other) = default;
			player(player&& other) = default;

			player& operator=(const player& other) = default;
			player& operator= (player&& other) = default;
				
			void update(float delta);
			void render();
				
			Camera3D& get_camera();
			void move_camera(int mode);

			void on_camera_move_event(const events::camera_move_event& event);
			rendering::frustrum& get_frustrum();

		private:
			void check_key_input(float delta);
			void build_huds();
			int camera_mode_;
			Camera3D camera_;
			const Vector3 camera_target_distance_;
			
			rendering::frustrum camera_frustrum_;
			events::event_handler<events::camera_move_event> camera_movement_handler_;

			/** the player holds the various huds that it would need i.e the ship and the pirate
			 * it draws the hud based on the index which changes upon detecting a certain event (docking / undocking)
			 */
			hud huds_[huds::size];
			size_t hud_index_;
	};
	
	class test_player{
		public:
		~test_player() = default;
		test_player()
		:camera_(Camera3D{}),  camera_mode_(CAMERA_THIRD_PERSON),
		 camera_frustrum_(camera_, ASPECT_RATIO, FOV, NEAR, FAR){
			camera_.position = Vector3Zero();;
			camera_.target = Vector3Zero(); // the camera looks at the cube, slightly above sea level
			camera_.up = Vector3{ 0.0, 1.0, 0.0 }; // rotation toward target
			camera_.fovy = FOV;
			camera_.projection = CAMERA_PERSPECTIVE; // should be third person mode ? 
		}
		test_player(const test_player& other)
			: camera_(other.camera_), camera_mode_(other.camera_mode_),
			camera_frustrum_(other.camera_frustrum_){
		};
				
		test_player(test_player&& other);

		test_player& operator=(const test_player& other) {
			camera_ = other.camera_;
			camera_mode_ = other.camera_mode_;
			camera_frustrum_ = other.camera_frustrum_;
			return *this;
		}
		
		void update(float delta);
		rendering::frustrum& get_frustrum();
		Camera3D& get_camera();
		private:
			Camera3D camera_;
			int camera_mode_;
			rendering::frustrum camera_frustrum_;
	};
}
#endif