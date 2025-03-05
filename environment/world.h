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
		: ocean_(Ocean()), player_(Player()) {
	};
	
	World(const World& other)
		: ocean_(other.ocean_), player_(other.player_) {
	};

	World(const World&& other);

	World& operator=(const World& ohter);
	World& operator = (const World && other);

	void update();
	void render();

	Player get_player();
	Ocean get_ocean();

private:

	// currently ocean and player, to be expanded upon
	Ocean ocean_;
	Player player_;
	std::vector<std::unique_ptr<Object>> world_objects_;
	float gravity_;
};