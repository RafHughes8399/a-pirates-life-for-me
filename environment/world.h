#pragma once
#include <vector>
#include <memory>

#include "raylib.h"
#include "ocean.h"
#include "land.h"
#include "../game/player.h"
#include "../game/managers.h"
#include "../objects/object.h"




// a functor is a class with an overloaded () operator that means the class can be called like a function
class AABBComparator {
public:
	bool operator()( const std::unique_ptr<Object>& a, const std::unique_ptr<Object>& b) {
		// based on the axis
		auto a_bb = GetModelBoundingBox(a->get_model());
		auto b_bb = GetModelBoundingBox(a->get_model());
		switch (axis_) {
		case 0:
			return a_bb.max.x < b_bb.min.x;
			break;
		case 1:
			return a_bb.max.y < b_bb.min.y;
			break;
		case 2:
			return a_bb.max.z < b_bb.min.y;
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
		: player_(Player()){
		// change this once the model manager is in play, and change the volume calculation
		world_objects_.push_back(std::make_unique<Object>(Object(Vector3{ 0.0f, -12.5f,0.0f },
			LoadModelFromMesh(GenMeshCube(1000.f, 25.0f, 1000.f)), WATER_DENISTY, 1000.0 * 25.0 * 1000.0)));
		auto ship = std::make_unique<Ship>(Ship(Vector3{ 0.0,0.0,0.0 }, LoadModelFromMesh(GenMeshCube(2.0f, 3.0f, 4.0f)), 15.0f, (2.0f * 4.5f * 4.0f), Vector3{ 0.0,0.0,0.0 }, Vector3{ 0.0,0.0,0.0 }, 0.0f));
		player_.set_ship(ship.get());
		world_objects_.push_back(std::move(ship));
		cmp_.axis_ = 0; // default axis of most variance is the x axis

	};
	
	World(const World& other)
		: player_(other.player_) {
		for (auto& o : world_objects_) {
			world_objects_.push_back(std::move(o));
		}
	};

	World(const World&& other);

	World& operator=(const World& ohter);
	World& operator = (const World && other);

	void update();
	void render();

	Player get_player();
private:
	Interaction_Manager interactor_;
	AABBComparator cmp_;
	void sort_objects();
	// currently ocean and player, to be expanded upon
	Player player_;
	std::vector<std::unique_ptr<Object>> world_objects_;
};