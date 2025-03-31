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
	bool operator()( const std::unique_ptr<Object>& a, const std::unique_ptr<Object>& b) {
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
	World()
		: player_(Player()), wind_(new Wind()){
		// change this once the model manager is in play, and change the volume calculation
		
		// init ocean
		world_objects_.push_back(std::make_unique<Ocean>(Ocean(ocean_type, 
			Vector3{0, WORLD_Y * -0.25, 0},
			Vector3{WORLD_X, WORLD_Y *0.5, WORLD_Z},
			WATER_DENISTY
			)));
		
		
		// init wind
		
		wind_->pick_direction();
		wind_->pick_speed();

		// init ship
		auto ship = std::make_unique<Ship>(Ship(ship_type,
			Vector3Zero(),
			Vector3{4.0f, 2.0f, 2.0f},
			SHIP_DENSITY,
			Vector3Zero(),
			PI / 2,
			wind_
			));
		// init player
		player_.set_ship(ship.get());
		world_objects_.push_back(std::move(ship));
		
		cmp_.axis_ = 0; // default axis of most variance is the x axis
	};
	World(const World& other)
		: player_(other.player_), wind_(other.wind_){
		for (auto& o : world_objects_) {
			world_objects_.push_back(std::move(o));
		}
	};

	World(const World&& other);

	World& operator=(const World& ohter);
	World& operator = (const World && other);

	void update();
	void render();
	
	Wind* get_wind();
	Player get_player();

private:
	void sort_objects();
	
	Vector3 world_size = Vector3{ WORLD_X, WORLD_Y, WORLD_Z };
	AABBComparator cmp_;

	Player player_;
	Wind* wind_;
	std::vector<std::unique_ptr<Object>> world_objects_;

	// temp, will move to a factory or whatever else
	ShipType& ship_type = ShipType::get_instance();
	OceanType& ocean_type = OceanType::get_instance();
	TerrainType& terrain_type = TerrainType::get_instance();
};