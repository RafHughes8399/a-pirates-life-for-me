#pragma once
#include <vector>
#include <memory>
#include <string>
#include <numbers>

#include "raylib.h"

#include "wind.h"
#include "player.h"
#include "object.h"
#include "chunk.h"



class World {
public:
	// CONSTRUCTORS
	World(Player& player)
		: wind_(new Wind()){
		// change this once the model manager is in play, and change the volume calculation
		wind_->pick_direction();
		wind_->pick_speed();
		
		generate_chunks();
		
		// init ocean
		world_objects_.push_back(std::make_unique<Ocean>(Ocean(ocean_type, 
			Vector3{0, WORLD_Y * -0.25, 0},
			Vector3{WORLD_X, WORLD_Y *0.5, WORLD_Z},
			WATER_DENISTY
			)));

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
		player.set_ship(ship.get());
		world_objects_.push_back(std::move(ship));
		
		cmp_.axis_ = 0; // default axis of most variance is the x axis
	};
	World(const World& other)
		: wind_(other.wind_){
		for (const auto& o : other.world_objects_) {
			world_objects_.push_back(o->clone());
		}
		cmp_.axis_ = other.cmp_.axis_;
	};

	World(const World&& other);

	World& operator=(const World& ohter);
	World& operator = (const World && other);

	void update(std::pair<int, int> player_chunk);
	void render(std::pair<int, int> player_chunk);
	
	Wind* get_wind();
	std::vector<std::unique_ptr<Object>>& get_objects();
private:
	void sort_objects();
	void generate_chunks();

	Vector3 world_size = Vector3{ WORLD_X, WORLD_Y, WORLD_Z };
	AABBComparator cmp_;

	Wind* wind_;
	std::vector<std::vector<Chunk>> chunks_;
	std::vector<std::unique_ptr<Object>> world_objects_;

	// temp, will move to a factory or whatever else
	ShipType& ship_type = ShipType::get_instance();
	OceanType& ocean_type = OceanType::get_instance();
	TerrainType& terrain_type = TerrainType::get_instance();
};