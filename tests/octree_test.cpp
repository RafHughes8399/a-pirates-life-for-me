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
	auto otree = octree(WORLD_BOX);

	// check is leaf
	CHECK(otree.is_leaf());
	// check num objects in the node 
	CHECK(otree.is_empty());
}
TEST_CASE("check tree subdivision is correctly done") {
	auto otree = octree(WORLD_BOX, 5);
	// check that the height is correct
	//CHECK(otree.height() == 5);
	
	// check the bounds of the node and its children



}
TEST_CASE("insert object into octree") {
	auto test_position = Vector3{ 200, 0.0f, -172.0f };
	auto test_size = Vector3{10, 10, 10};
	std::unique_ptr<Object> test_obj = std::make_unique<TestObject>(test_position, test_size);

	// create an empty octree, insert the object into the tree

	auto tree = octree(WORLD_BOX);
	tree.insert(test_obj);

	// check it was inserted
	CHECK(tree.size() == 1);

	// check it was inserted into the "correct" node, i.e confirm the placement
	// find object node
	// find object (that is, contains)

	auto node = tree.find_object_node(test_obj);
	CHECK(node != nullptr); // the object is located in a node in the tree 
	

	/**
	 * a object is correctly placed if it fits in a node but not its children
	 * this indicates that it is in the smallest possible bound
	 */
	CHECK(tree.object_in_node(node->bounds_, test_obj->get_bounding_box()));
	for (auto& child : node->children_) {
		CHECK(! tree.object_in_node(child->bounds_, test_obj->get_bounding_box()));
	}
}

TEST_CASE("insert objects into octree") {

}

TEST_CASE("remove an object from the tree ") {

}
TEST_CASE("remove all objects from tree") {

}
TEST_CASE("check tree height") {

}

TEST_CASE("check tree size") {

}

TEST_CASE("reposition an object in the tree") {

}


