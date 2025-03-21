#include "world.h"
#include "raymath.h"
#include <algorithm>

float get_maximal_variance_axis(Vector3 bb, int axis) {
	switch (axis) {
		case 0: 
			return bb.x;
			break;
		case 1:
			return bb.y;
			break;
		case 2:
			return bb.z;
			break;
	}
}

void World::sort_objects() {
	std::sort(world_objects_.begin(), world_objects_.end(), cmp_); // add the comparator, if this does not work, just define < for Object, default axis is x
	auto centre_sum = Vector3{0.0f, 0.0f, 0.0f};
	auto centre_square_sum = Vector3{0.0f, 0.0f, 0.0f };

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
			auto current_max = get_maximal_variance_axis(current_bounding_box.max, cmp_.axis_);
			auto other_min = get_maximal_variance_axis(other_bounding_box.min, cmp_.axis_);
			if (other_min < current_max){
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

void World::update(){
	// check for interactions 
	auto time = GetTime();
		sort_objects();
	// then update objects - changing positions, states, etc .
	// for each object check if it intersects 
	for (auto& o : world_objects_) {
		o->update();
	}
	player_.update();

	// randomise the wind every 90 seconds ? 
	wind_.update(time);
}

void World::render() {
	DrawGrid(50, 1);
	for (auto& o : world_objects_) {
		o->render();
	}
	player_.render();
}

Wind World::get_wind(){
	return wind_;
}

Player World::get_player(){
	return player_;
}
