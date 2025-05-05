#include "world.h"
#include "raymath.h"
#include "utility_functions.h"
#include <algorithm>


void World::sort_objects() {
	std::sort(world_objects_.begin(), world_objects_.end(), cmp_); // add the comparator, if this does not work, just define < for Object, default axis is x
	auto centre_sum = Vector3{ 0.0f, 0.0f, 0.0f };
	auto centre_square_sum = Vector3{ 0.0f, 0.0f, 0.0f };

	for (auto i = 0; i < world_objects_.size() - 1; ++i) {
		// get the centre of the bounding box 
		auto current_bounding_box = world_objects_[i]->get_bounding_box();
		auto centre = (current_bounding_box.max + current_bounding_box.min) / 2;
		centre_sum += centre;
		centre_square_sum += centre * centre;

		// find intersections for the current entity
		for (auto j = i + 1; j < world_objects_.size(); ++j) {
			auto other_bounding_box = world_objects_[j]->get_bounding_box();
			// compare if other_bb is in the interval of current_bb, on the axis of max variance
			// so if current. max < other.min, then intersection
			auto current_max = utility::get_maximal_variance_axis(current_bounding_box.max, cmp_.axis_);
			auto other_min = utility::get_maximal_variance_axis(other_bounding_box.min, cmp_.axis_);
			if (other_min < current_max) {
				// check for a more accurate intersection between the two objects

				// for now a simple shape collision
				if (CheckCollisionBoxes(current_bounding_box, other_bounding_box)) {
					// and the interaction has not happened yet
					world_objects_[i]->interact(world_objects_[j].get());
				}
			}
			else {
				// otherwise if not, then no other object will intersect with this one
				break;
			}

		}
	}
	// set max variance axis
	centre_sum /= world_objects_.size();
	centre_square_sum /= world_objects_.size();
	auto variance = centre_square_sum - (centre_sum * centre_sum);
	auto maximal_axis = variance.x;
	cmp_.axis_ = 0;
	if (variance.y > maximal_axis) {
		maximal_axis = variance.y;
		cmp_.axis_ = 1;
	}
	if (variance.z > maximal_axis) {
		maximal_axis = variance.z;
		cmp_.axis_ = 2;
	}

}

void World::generate_islands(){
	// for now, just generate the underlying terrain for all the islands in the game 
	// TODO: position them
	// TODO: calculate their bounding boxes
	// TODO: texture them

	// the object constructor is 
	// obj type,
	// position
	// size
	// min
	// max
	// density.
	
	auto hub_position = Vector3{ -80.0f, -4.3f, 50.0f};

	auto hub = std::make_shared<Terrain>(Terrain(
		HubType::get_instance(),
		hub_position,
		Vector3{100, 25, 135},
		Vector3{hub_position.x - 70.0f, hub_position.y + 1.6f, hub_position.z  - 75.0f},
		Vector3{hub_position.x + 75.0f, hub_position.y + 14, hub_position.z + 25.0f},
		100			
	));
	auto lagoon_position = Vector3{ -225.0f, -4.8f, 325.0f };
	auto lagoon = std::make_shared<Terrain>(Terrain(
		LagoonType::get_instance(),
		lagoon_position,
		Vector3{ 125, 25, 125 },
		Vector3{lagoon_position.x - 80.0f, lagoon_position.y - 0.2f, lagoon_position.z - 75.0f},
		Vector3{lagoon_position.x + 80.0f, lagoon_position.y + 14.2f, lagoon_position.z + 70.0f},
		100
	));

	// make the reef
	auto reef_position = Vector3{75.0f, -1.2f, 500};
	auto reef = std::make_shared<Terrain>(Terrain(
		ReefType::get_instance(),
		reef_position,
		Vector3{ 100, 100, 100 },
		Vector3{reef_position.x - 40.0f, reef_position.y - 1.5f, reef_position.z - 90.0f},
		Vector3{reef_position.x + 39.0f, reef_position.y + 4.5f, reef_position.z + 100.0f},
		100
	));
	auto bay_position = Vector3{300, -1.0f, 400};
	auto bay = std::make_shared<Terrain>(Terrain(
		BayType::get_instance(),
		bay_position,
		Vector3{ 100, 10, 100 },
		Vector3{bay_position.x - 94.9f, bay_position.y -1.0f, bay_position.z - 63.0f},
		Vector3{bay_position.x + 101.5f, bay_position.y + 11.2f, bay_position.z + 66.2f},
		100
	));


	auto min = Vector3{191, -1, -432};
	auto max = Vector3{420, 9.2, -256};
	auto cove_position = Vector3{315, -1.4f, -350};
	auto cove = std::make_shared<Terrain>(Terrain(
		CoveType::get_instance(),
		cove_position,
		Vector3{100, 10, 100},
		Vector3{cove_position.x - 114 , cove_position.y -1.2f, cove_position.z - 82},
		Vector3{cove_position.x + 125, cove_position.y + 11.2f, cove_position.z + 94},
		100
	));

	// make the isle
	auto isle_position = Vector3{-275, -0.3f, -280};
	auto isle = std::make_shared<Terrain>(Terrain(
		IsleType::get_instance(),
		isle_position,
		Vector3{100, 10, 100},
		Vector3{isle_position.x - 60.5f, isle_position.y - 1.7f, isle_position.z - 102.5f},
		Vector3{isle_position.x + 49.5f, isle_position.y + 8.2f, isle_position.z + 116.5f},
		100
	));

	// make the bay
	world_objects_.push_back(hub);
	world_objects_.push_back(lagoon);
	world_objects_.push_back(reef);
	world_objects_.push_back(bay);
	world_objects_.push_back(isle);
	world_objects_.push_back(cove);
	/**
	*/

}

void World::update(){
	// based on player position, update based on simulation distance
	// check for interactions 
	World::sort_objects();
	auto delta = GetFrameTime();
	for (auto& o : world_objects_) {
		o->update(delta);
	}
	// randomise the wind every 90 seconds ? 
	wind_.update(GetTime());
}

void World::render(BoundingBox& camera_view_box) {
	// based on the player position, render based on render distnace
	DrawGrid(NUM_CHUNKS, CHUNK_SIZE);

	// TODO for debug, draw the chunk bounding boxes

	// make sure you update before render, so that its sorted when you render

	// ok it is sorted
	for (auto& o : world_objects_) {
		// check the distance and the direction from the camera
		o->render();
	}
}

std::vector<std::shared_ptr<Object>>& World::get_objects(){
	// TODO: insert return statement here
	return world_objects_;
}
