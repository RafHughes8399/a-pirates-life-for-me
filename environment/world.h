#pragma once
#include "raylib.h"
#include "Ocean.h"
#include "land.h"
#include "../game/Player.h"
// ok ok the World, what does the World have

// well the World has 
class World {
public:
	// CONSTRUCTORS
	World();
	
	World(const World& other);

	World(const World&& other);

	World& operator=(const World& ohter);
	World& operator = (const World && other);

	void update();
	void render();

private:
	Ocean Ocean_;
	// the World has a Ship
	Player Player_;
};