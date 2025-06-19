#include "../lib/catch2/catch.hpp"
#include "../src/octree.h"
#include "../src/config.h"
#include "../src/object.h"

#include <memory>
#include <vector>

#define WORLD_MIN Vector3{(WORLD_X / 2) * -1, (WORLD_Y /2) * -1, (WORLD_Z / 2) * -1}
#define WORLD_MAX Vector3{WORLD_X / 2, WORLD_Y / 2, WORLD_Z / 2}
#define WORLD_BOX BoundingBox{WORLD_MIN, WORLD_MAX}


std::vector<std::unique_ptr<Object>> build_test_objects() {
	return std::vector<std::unique_ptr<Object>>{};
}



TEST_CASE("empty octree construction") {
	// build octree 
	auto otree = octree<Object>(WORLD_BOX);

	// check is leaf
	CHECK(otree.is_leaf());
	// check num objects in the node 
	CHECK(otree.is_empty());
}
TEST_CASE("insert object into octree") {
	auto test_position = Vector3{ 200, 0.0f, -172.0f };
	auto test_size = Vector3{10, 10, 10};
	std::unique_ptr<Object> test_obj = std::make_unique<TestObject>(test_position, test_size);

	// create an empty octree, insert the object into the tree

	auto tree = octree<Object>(WORLD_BOX);
	tree.insert(test_obj);

	// check it was inserted
	//CHECK(tree.size() == 1);
	

	// check it was inserted into the "correct" node
}
