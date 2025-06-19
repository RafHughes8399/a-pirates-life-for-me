#include "../lib/catch2/catch.hpp"
#include "../src/octree.h"
#include "../src/config.h"

#include <memory>
#include <vector>

#define WORLD_MIN Vector3{(WORLD_X / 2) * -1, (WORLD_Y /2) * -1, (WORLD_Z / 2) * -1}
#define WORLD_MAX Vector3{WORLD_X / 2, WORLD_Y / 2, WORLD_Z / 2}
#define WORLD_BOX BoundingBox{WORLD_MIN, WORLD_MAX}

using obj = std::unique_ptr<Object>; 

std::vector<std::unique_ptr<Object>> build_test_objects() {
	return std::vector<std::unique_ptr<Object>>{};
}



TEST_CASE("empty octree construction") {
	// build octree 
	auto otree = octree<obj>(WORLD_BOX);

	// check is leaf
	CHECK(otree.is_leaf());
	// check num objects in the node 
	CHECK(otree.is_empty());
}
