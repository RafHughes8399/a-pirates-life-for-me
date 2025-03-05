#pragma once
#include <vector>
#include <memory>

#include "raylib.h"
#include "ocean.h"
#include "land.h"
#include "../game/player.h"
#include "../objects/object.h"
class World {
public:
	// CONSTRUCTORS
	World()
		: player_(Player()) {
		// change this once the model manager is in play, and change the volume calculation
		world_objects_.push_back(std::make_unique<Object>(Object(Vector3{ 0.0f, -12.5f,0.0f },
			LoadModelFromMesh(GenMeshCube(1000.f, 25.0f, 1000.f)), WATER_DENISTY, 1000.0 * 25.0 * 1000.0)));
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
	Object* get_ocean();

private:

	// currently ocean and player, to be expanded upon
	Player player_;
	std::vector<std::unique_ptr<Object>> world_objects_;
};