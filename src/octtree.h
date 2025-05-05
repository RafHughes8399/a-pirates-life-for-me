#pragma once
#include <vector>
#include <queue>
#include "object.h"


#define  MIN_SIZE 1 // the smallest cube will be a 1 x 1 x 1 region
#define LIFESPAN 64 // lifespan of a node before it is culled 

using ObjectList = std::vector<std::shared_ptr<Object>>;
using Queue = std::queue<std::shared_ptr<Object>>;

class OctTree {
public:
	OctTree()
		:region_(BoundingBox{ Vector3Zero(), Vector3Zero() }),
			objects_({}), current_life_(-1){};
	OctTree(BoundingBox region)
			: region_(region), objects_({}), current_life_(-1) {
		};


		// lets define some functions
	void build_tree();
	void insert(std::shared_ptr<Object>);
	std::shared_ptr<Object> remove(std::shared_ptr<Object>);
private:
	OctTree(BoundingBox region, ObjectList objects)
		: region_(region), objects_(objects), current_life_(-1) {
	};



	BoundingBox region_;
	ObjectList objects_;
	OctTree* parent_;
	OctTree* children_[8];
	int current_life_ = -1;

	};

	/**
			each node has a bounding region
			each node has a reference to the parent
			each node contains an array of eight children
			contains a list of objects within the region


			static variables to indicate tree state

			build the tree, identify its regions
			insert objects, remove them

			update and render the game based on tthe






	*/