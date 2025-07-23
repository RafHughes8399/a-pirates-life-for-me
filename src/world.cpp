#include "environment.h"
#include "utility_functions.h"
#include <algorithm>

#define HUB Vector3{ -80.0f, -4.3f, 50.0f}
#define LAGOON Vector3{ -225.0f, -4.8f, 325.0f }
#define REEF Vector3{75.0f, -1.2f, 500}
#define BAY Vector3{300, -1.0f, 400}
#define COVE Vector3{315, -1.4f, -350}
#define ISLE Vector3{-275, -0.3f, -280}
void environment::world::build_world(wind& wind, player::player& player){
	// build the ocean

	std::cout << "building the world " << std::endl;
	std::unique_ptr<entities::entity> ocean = std::make_unique<entities::ocean>(
		OceanType::get_instance(),
		WORLD_CENTRE,
		WORLD_MIN,
		WORLD_MAX,
		world_entities_.get_next_id()
	);
	world_entities_.insert(ocean);
	// build the ship
	std::unique_ptr<entities::entity> player_ship = std::make_unique<entities::ship>(
		ShipType::get_instance(),
		SHIP_START,
		Vector3{SHIP_START.x -1.0f ,SHIP_START.y, SHIP_START.z -1.0f},
		Vector3{SHIP_START.x + 1.6f, SHIP_START.y + 2.8f, SHIP_START.z + 1.6f},
		world_entities_.get_next_id()
	);

	//let the player ship subscribe to the wind to listen for updates 
	auto player_ship_ptr = static_cast<entities::ship*>(player_ship.get());
	wind_.add_ship_subscriber(player_ship_ptr);
	//then set the player ship pointer, so it can be tracked
	player.set_ship(player_ship_ptr);

	world_entities_.insert(player_ship);
	// build the islands

	generate_islands();
}
void environment::world::generate_islands(){
	// for now, just generate the underlying terrain for all the islands in the game 
	// TODO: position them
	// TODO: calculate their bounding boxes
	// TODO: texture them

	// the object constructor is, i think size and density can go, they seem not so necessary
	// obj type,
	// position
	// min
	// max
	// id, which you know.
	
	std::unique_ptr<entities::entity> hub = std::make_unique<entities::terrain>(entities::terrain(
		HubType::get_instance(),
		HUB,
		Vector3{HUB.x - 70.0f, HUB.y + 1.6f, HUB.z  - 75.0f},
		Vector3{HUB.x + 75.0f, HUB.y + 14, HUB.z + 25.0f},
		world_entities_.get_next_id()		
	));

	std::unique_ptr<entities::entity> lagoon = std::make_unique<entities::terrain>(entities::terrain(
		LagoonType::get_instance(),
		LAGOON,
		Vector3{LAGOON.x - 80.0f, LAGOON.y - 0.2f, LAGOON.z - 75.0f},
		Vector3{LAGOON.x + 80.0f, LAGOON.y + 14.2f, LAGOON.z + 70.0f},
		world_entities_.get_next_id()
	));

	// make the reef
	std::unique_ptr<entities::entity> reef = std::make_unique<entities::terrain>(entities::terrain(
		ReefType::get_instance(),
		REEF,
		Vector3{REEF.x - 40.0f, REEF.y - 1.5f, REEF.z - 90.0f},
		Vector3{REEF.x + 39.0f, REEF.y + 4.5f, REEF.z + 100.0f},
		world_entities_.get_next_id()
	));
	std::unique_ptr<entities::entity> bay = std::make_unique<entities::terrain>(entities::terrain(
		BayType::get_instance(),
		BAY,
		Vector3{BAY.x - 94.9f, BAY.y -1.0f, BAY.z - 63.0f},
		Vector3{BAY.x + 101.5f, BAY.y + 11.2f, BAY.z + 66.2f},
		100
	));

	std::unique_ptr<entities::entity> cove = std::make_unique<entities::terrain>(entities::terrain(
		CoveType::get_instance(),
		COVE,
		Vector3{COVE.x - 114 , COVE.y -1.2f, COVE.z - 82},
		Vector3{COVE.x + 125, COVE.y + 11.2f, COVE.z + 94},
		world_entities_.get_next_id()
	));

	// make the isle
	std::unique_ptr<entities::entity> isle = std::make_unique<entities::terrain>(entities::terrain(
		IsleType::get_instance(),
		ISLE,
		Vector3{ISLE.x - 60.5f, ISLE.y - 1.7f, ISLE.z - 102.5f},
		Vector3{ISLE.x + 49.5f, ISLE.y + 8.2f, ISLE.z + 116.5f},
		world_entities_.get_next_id()
	));

	// make the bay
	world_entities_.insert(hub);
	world_entities_.insert(lagoon);
	world_entities_.insert(reef);
	world_entities_.insert(bay);
	world_entities_.insert(isle);
	world_entities_.insert(cove);
}

void environment::world::build_frustrum_test_world(){
	
	std::cout << "building the world " << std::endl;
	// generate a bunch of test objects, objects are 2, 2 , 2
	// 2 dimensional loop silly boy, ok wait thats far too many cubes, literally half a million
	for(float i = WORLD_MIN.x + 5; i < WORLD_MAX.x - 5; i += 20){
		for(float j = WORLD_MIN.z + 5; j < WORLD_MAX.z -5; j += 20){
			std::unique_ptr<entities::entity> cube = std::make_unique<entities::test_entity>(
				TestType::get_instance(),
				Vector3{i, 5, j},
				Vector3{i - 2.5, 0, j - 2.5},
				Vector3{i + 2.5, 0, j + 2.5},
				world_entities_.get_next_id()
			);
			world_entities_.insert(cube);
		}
	}
}

void environment::world::update(){
	// based on player position, update based on simulation distance
	// check for interactions 
	auto delta = GetFrameTime();
	world_entities_.update(delta);

	// randomise the wind every 90 seconds ? 
	wind_.update(GetTime());
}

void environment::world::render(rendering::frustrum& rendering_frustrum) {
	// TODO: frustrum culling, for now just render everything in the tree 
	// pass the frustrum in, check objects against it
	auto num_rendered = 0;
	// this is ineffective, figure out another way
	auto w_entities = world_entities_.get_objects();
	for(auto & entity : w_entities){
		// check if the entitiy
		if(rendering_frustrum.contains(entity.get()->get_bounding_box())){
			entity.get()->render();
			num_rendered++;
		}

	}
	// quick debug to check if this is working, i think not because its rendering everything
	// do some more debug printing here
	
	
	std::cout << "total objects: " << w_entities.size() << " || objects rendered: " << num_rendered << std::endl;
}
