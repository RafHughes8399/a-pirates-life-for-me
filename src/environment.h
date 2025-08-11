#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
// std lib includes
#include <vector>
#include <memory>
#include <string>
#include <numbers>

// raylib includes 

// game includes
#include "player.h"
#include "entities.h"
#include "octree.h"
#include "config.h"

// defines
#define WORLD_MIN Vector3Scale(Vector3{WORLD_X, WORLD_Y, WORLD_Z}, -0.5)
#define WORLD_MAX Vector3Scale(Vector3{WORLD_X, WORLD_Y, WORLD_Z}, 0.5)
#define WORLD_CENTRE Vector3Scale(Vector3Add(WORLD_MIN, WORLD_MAX), 0.5)

#define WORLD_BOX BoundingBox{WORLD_MIN, WORLD_MAX}



namespace environment{
class wind {
	public:
		~wind() = default;
		// default constructor
		wind()
			: direction_(0.0f), speed_(0.0f){
		};
		wind(float direction, float speed)
			:direction_(direction), speed_(speed) {
		};
		wind(const wind& other)
			: direction_(other.direction_), speed_(other.speed_), time_randomised_(other.time_randomised_),
			ship_subscribers_(other.ship_subscribers_){
		};
		wind(wind&& other)
			:direction_(std::move(other.direction_)), speed_(std::move(other.speed_)), time_randomised_(std::move(other.time_randomised_)) {
		};

		float get_speed() const;
		float get_direction() const;

		void pick_speed();
		void pick_direction();

		void update_time(double time);
		void update(double time);
		Vector3 get_direction_coefficient();

		void add_ship_subscriber(entities::player_ship* & player_ship);
		void remove_ship_subscriber(entities::player_ship* & player_ship);
		void notify_ships();
		void notify_ship(size_t ship_);

	private:
		std::vector<entities::player_ship*> ship_subscribers_;
		float direction_;
		float speed_; 
		double time_randomised_ = WIND_CHANGE_TIME;
	};
class world {
	public:
		// CONSTRUCTORS
		world(player::player& player)
		: wind_(wind()), world_entities_(tree::octree(WORLD_BOX)){
			build_world(wind_, player);
		}

		world(player::test_player& player)
		: wind_(wind()), world_entities_(tree::octree(WORLD_BOX)){
			build_frustrum_test_world();
		}
		world(const world& other)
		: wind_(other.wind_), world_entities_(other.world_entities_){};
		
		world(const world&& other);
		
		world& operator=(const world& other);
		world& operator= (const world && other);
		
		void update(float delta);
		void render(rendering::frustrum& rendering_frustrum);
		
		entities::entity* get_entity(int id);
		void populate_test_objects();
	private:
		void generate_islands();
		void build_world(wind& wind, player::player& player);
		void build_frustrum_test_world();
		wind wind_;
		tree::octree world_entities_;
		
	};
}
#endif