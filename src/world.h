
// std lib includes
#include <vector>
#include <memory>
#include <string>
#include <numbers>

// raylib includes 

// game includes
#include "wind.h"
#include "player.h"
#include "object.h"
#include "octree.h"
#include "config.h"

// defines
#define WORLD_MIN Vector3Scale(Vector3{WORLD_X, WORLD_Y, WORLD_Z}, -0.5)
#define WORLD_MAX Vector3Scale(Vector3{WORLD_X, WORLD_Y, WORLD_Z}, 0.5)
#define WORLD_BOX BoundingBox{WORLD_MIN, WORLD_MAX}

// a functor is a class with an overloaded () operator that means the class can be called like a function
class World {
public:
	// CONSTRUCTORS
	World(Player& player)
		: wind_(Wind()), world_entities_(tree::octree(WORLD_BOX)){}
/* 		// init oceant
		world_objects_.push_back(std::make_shared<Ocean>(Ocean(OceanType::get_instance(), 
			Vector3{0, WORLD_Y * -0.25, 0},
			Vector3{WORLD_X, WORLD_Y *0.5, WORLD_Z},
			WATER_DENISTY
			)));

		generate_islands();
		wind_.pick_direction();
		wind_.pick_speed();

		// init ship
		auto ship_position = Vector3{200, 0.0f, -172.0f};
		auto ship = std::make_shared<Ship>(Ship(ShipType::get_instance(),
			ship_position,
			Vector3{ 0.75f, 0.5f, 1.2 },
			Vector3{ship_position.x -1.0f ,ship_position.y, ship_position.z -1.0f},
			Vector3{ship_position.x + 1.6f, ship_position.y + 2.8f, ship_position.z + 1.6f},
			SHIP_DENSITY,
			Vector3Zero(),
			PI / 2));

		std::weak_ptr<Ship> ship_weak = ship;	
		wind_.add_ship_subscriber(ship_weak);
		wind_.notify_ship(0);	

		// init player
		player.set_ship(ship.get());
		world_objects_.push_back(ship); */
	World(const World& other)
		: wind_(other.wind_), world_entities_(other.world_entities_){};

	World(const World&& other);

	World& operator=(const World& other);
	World& operator= (const World && other);

	void update();
	void render(BoundingBox& camera_view_box);
	
private:
	void generate_islands();

	Wind wind_;
	tree::octree world_entities_;
	
};