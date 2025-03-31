#pragma once
#include <set>
#include "raylib.h"

// as a data structure i want really quick access and lookup
// insertion doesn't really matter 
class BoundingBoxComparator {
	BoundingBoxComparator();
	bool operator()(BoundingBox a, BoundingBox b) {
		// compare the min and max of the bounding boxes 
	}

};
class Hitbox {
public:
	~Hitbox() = default;
	Hitbox(); 
	Hitbox(const Hitbox& other);
	Hitbox(Hitbox&& other);

	// unsure of the parameters, cook a bit more on the mehtods that you need
	BoundingBox get_overall_box();
	std::set<BoundingBox>& get_bounding_boxes();
	// how the other hitbox intersects with this hitbox
	bool intersects_with_hitbox(Hitbox& other);


private:
	BoundingBox overall_box_;
	std::set<BoundingBox> bounding_boxes_;
};