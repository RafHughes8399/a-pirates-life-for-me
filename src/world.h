#pragma once
#include <vector>
#include <memory>
#include <string>
#include <numbers>

#include "raylib.h"

#include "wind.h"
#include "player.h"
#include "object.h"



// a functor is a class with an overloaded () operator that means the class can be called like a function
class AABBComparator {
public:
	bool operator()( const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
		// based on the axis
		auto a_bb = GetModelBoundingBox(a->get_model());
		auto b_bb = GetModelBoundingBox(b->get_model());
		switch (axis_) {
		case 0:
			return a_bb.max.x <= b_bb.min.x;
			break;
		case 1:
			return a_bb.max.y <= b_bb.min.y;
			break;
		case 2:
			return a_bb.max.z <= b_bb.min.y;
			break;
		default:
			return true;
			break;
		}
	}
	int axis_;

};
class World {
public:
	// CONSTRUCTORS
	World(Player& player)
		: wind_(Wind()){
		
		// init oceant
		world_objects_.push_back(std::make_shared<Ocean>(Ocean(ocean_type, 
			Vector3{0, WORLD_Y * -0.25, 0},
			Vector3{WORLD_X, WORLD_Y *0.5, WORLD_Z},
			WATER_DENISTY
			)));

		generate_islands();
		wind_.pick_direction();
		wind_.pick_speed();

		// init ship
		auto ship = std::make_shared<Ship>(Ship(ship_type,
			Vector3Zero(),
			Vector3{ 0.75f, 0.5f, 1.2 },
			Vector3{-1.0 ,0.0, -1.0},
			Vector3{1.6, 2.8, 1.6},
			SHIP_DENSITY,
			Vector3Zero(),
			PI / 2));

		std::weak_ptr<Ship> ship_weak = ship;	
		wind_.add_ship_subscriber(ship_weak);
		wind_.notify_ship(0);	

		// init player
		player.set_ship(ship.get());
		world_objects_.push_back(ship);
		
		cmp_.axis_ = 0; // default axis of most variance is the x axis
	};
	World(const World& other)
		: wind_(other.wind_){
		for (auto& o : other.world_objects_) {
			world_objects_.push_back(o);
		}
		cmp_.axis_ = other.cmp_.axis_;
	};

	World(const World&& other);

	World& operator=(const World& ohter);
	World& operator= (const World && other);

	void update();
	void render();
	
	std::vector<std::shared_ptr<Object>>& get_objects();
private:
	void sort_objects();
	void generate_islands();
	Vector3 world_size = Vector3{ WORLD_X, WORLD_Y, WORLD_Z };
	AABBComparator cmp_;

	Wind wind_;
	std::vector<std::shared_ptr<Object>> world_objects_;

	// temp, will move to a factory or whatever else
	ShipType& ship_type = ShipType::get_instance();
	OceanType& ocean_type = OceanType::get_instance();
	TerrainType& terrain_type = TerrainType::get_instance();
};