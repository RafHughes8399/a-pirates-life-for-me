
#include <vector>
#include <memory>
#include "object.h"
#include "raylib.h"
#define NODE_LIFETIME 30 // seconds
#define MAX_DEPTH 15 // max height of the tree 
class octree {
private:
	// node definition
	struct node {
		std::vector<std::unique_ptr<Object>> objects_;
		std::vector<std::unique_ptr<node>> children_;
		BoundingBox bounds_;
		int depth_;
		short life_; // how long a node has lived without any objects
	};

	// members
	std::unique_ptr<node> root_;
	int max_depth_;
	// methods

	bool node_contains_object(BoundingBox& node, BoundingBox& object) {
		return (object.min.x > node.min.x and object.min.y > node.min.y and object.min.z > node.min.z)
			and (object.max.x < node.max.x and object.max.y < node.max.y and object.max.z < node.max.z);
	}
	// assumes that the current node has no children
	void build_children(std::unique_ptr<node>& tree) {
		// first do quadrants then do octant	
		// left or right, bottom or top, front or back
		auto centre_point = Vector3Add(tree->bounds_.max, tree->bounds_.min);
		centre_point = Vector3Scale(centre_point, 0.5);

		// left bottom front, (min -> centre x, y) (centre -> max z)
		auto lbf = std::make_unique<node>();
		lbf->bounds_ = BoundingBox{Vector3{tree->bounds_.min.x, tree->bounds_.min.y, centre_point.z},
			Vector3{centre_point.x, centre_point.y, tree->bounds_.max.z}};
		lbf->life_ = 0;
		lbf->depth_ = tree->depth_ + 1;
		tree->children_.push_back(std::move(lbf));

		// left top front (min -> centre x), (centre -> max y, z)
		auto ltf = std::make_unique<node>();
		ltf->bounds_ = BoundingBox{Vector3{tree->bounds_.min.x, centre_point.y, centre_point.z},
			Vector3{centre_point.x, tree->bounds_.max.y, tree->bounds_.max.z}
		};
		ltf->life_ = 0;
		ltf->depth_ = tree->depth_ + 1;
		tree->children_.push_back(std::move(ltf));

		// left bottom back (min -> centre x, y, z)
		auto lbb = std::make_unique<node>();
		lbb->bounds_ = BoundingBox{tree->bounds_.min, centre_point};
		lbb->life_ = 0;
		lbb->depth_ = tree->depth_ + 1;
		tree->children_.push_back(std::move(lbb));

		// left top back (min -> centre x, z) (centre -> max, y)
		auto ltb = std::make_unique<node>();
		ltb->bounds_ = BoundingBox{Vector3{tree->bounds_.min.x, centre_point.y, tree->bounds_.min.z},
			Vector3{centre_point.x, tree->bounds_.max.y, centre_point.z}
		};
		ltb->life_ = 0;
		ltb->depth_ = tree->depth_ + 1;
		tree->children_.push_back(std::move(ltb));

		// right bottom front (min -> centre y) (centre -> max x, z)
		auto rbf = std::make_unique<node>();
		rbf->bounds_ = BoundingBox{Vector3{centre_point.x, tree->bounds_.min.y, centre_point.z}, 
			Vector3{tree->bounds_.max.x, centre_point.y, tree->bounds_.max.z}
		};
		rbf->life_ = 0;
		rbf->depth_ = tree->depth_ + 1;
		tree->children_.push_back(std::move(rbf));

		// right top front (centre -> max x, y, z)
		auto rtf = std::make_unique<node>();
		rtf->bounds_ = BoundingBox{centre_point, tree->bounds_.max};
		rtf->life_ = 0;
		rtf->depth_ = tree->depth_ + 1;
		tree->children_.push_back(std::move(rtf));

		// right bottom back (min -> centre y, z) (centre -> max, x)
		auto rbb = std::make_unique<node>();
		rbb->bounds_ = BoundingBox{Vector3{centre_point.x, tree->bounds_.min.y, tree->bounds_.min.z}, 
			Vector3{tree->bounds_.max.x, centre_point.y, centre_point.z} };
		rbb->life_ = 0;
		rbb->depth_ = tree->depth_ + 1;
		tree->children_.push_back(std::move(rbb));

		// right top back (min -> centre z) ( centre -> max x y)
		auto rtb = std::make_unique<node>();
		rtb->bounds_ = BoundingBox{ Vector3{centre_point.x, centre_point.y, tree->bounds_.min.z},
		Vector3{tree->bounds_.max.x, tree->bounds_.max.y, centre_point.z} };
		rtb->life_ = 0;
		rtb->depth_ = tree->depth_ + 1;
		tree->children_.push_back(std::move(rtb));
	}
	
	// insert and remove a Object from the octree
	void insert(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object) {
		// ok so what is the insert logic
		// find the smallest node that the object fits in and insert it into the vector
		// such that the max depth of the tree is not exceeded
		

		// if fits in parent, check the children
		auto object_bounds = object->get_bounding_box();
		if (node_contains_object(tree->bounds_, object_bounds)) {
			// if there are no children, check depth, if node is not at the max dpeth, generate the children
			if (is_leaf(tree) && tree->depth_ != max_depth_) {
				build_children(tree);
				// then check the children,  
			}
			else if (tree->depth_ == max_depth_) {
				tree->objects_.push_back(std::move(object));
				return;
			}
			// then check the children
			for (auto& child : tree->children_) {
				// if does fit in a child, recursively
				if (node_contains_object(child->bounds_, object_bounds)) {
					insert(child, object);
					return;
				}
			}
			// otherwise insert into current node
			tree->objects_.push_back(std::move(object));
		}
		// if does not fit in node do not insert
		return;
	}
	void erase(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object) {
		return;
	}

	// reposition an Object within the tree after it moves
	std::unique_ptr<Object> extract(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object) {
		return nullptr;
	}
	void reposition(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object) {
		return;
	}

	// get the node the object is located in 
	node* find_object_node(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object) {
		if (!tree) {
			return nullptr;
		}
		for (auto& obj : tree->objects_) {
			if (obj.get() == object.get()) {
				return tree.get();
			}
		}
		for (auto& child : tree->children_) {
			auto result =  find_object_node(child, object);
			if (result != nullptr) {
				return result;
			}
		}
		return nullptr;
	}

	// same logic but returns the object instead of the node 
	Object* find_object(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object) {
		if (!tree) return nullptr;

		// Check if object is in current node
		for (auto& obj : tree->objects_) {
			if (obj.get() == object.get()) {  // Compare raw pointers
				return obj.get();
			}
		}

		// Recursively search children
		for (auto& child : tree->children_) {
			auto result = find_object(child, object);
			if (result != nullptr) {
				return result;
			}
		}

		return nullptr;  // Not found
	}

	int height(std::unique_ptr<node>& tree) {
		if (!tree) {
			return -1;
		}
		else {
			int max_child_height = -1;
			for (auto& child : tree->children_) {
				int child_height = height(child);
				if (child_height >= max_child_height) {
					max_child_height = child_height;
				}
			}
			return 1 + max_child_height;
		}
	}
	int size(std::unique_ptr<node>& tree) {
		auto empty = is_empty(tree);
		if (empty) { return 0; }

		if (tree != nullptr) {
			auto t_size = tree->objects_.size();
			for (auto& child : tree->children_) {
				t_size += size(child);
			}
			return t_size;
		}
		return 0;
	}


	bool is_empty(std::unique_ptr<node>& tree) {
		// check the current list 
		auto empty = tree->objects_.size() == 0 ? true : false;
		// check the children
		if (! empty) { return false; }
		for (auto& child : tree->children_) {
			if (not is_empty(child)) {
				return false;
			}
		}
		return true;
	}
	bool is_leaf(std::unique_ptr<node>& tree) {
		return tree->children_.size() == 0 ? true : false;
	}

	void check_leaves(std::unique_ptr<node>& tree, double delta) {
			if (is_leaf(tree)) {
				tree->life_ += delta;
				if (tree->life_ > NODE_LIFETIME) {
					tree.reset();
					return;
				}
			}
			else {
				for (auto& child : tree->children_) {
					check_leaves(child, delta);
				}
			}
		return;
	} 

public:
// CONSTRUCTORS
	~octree() = default;
	// creates an empty octree with a root node
	octree(BoundingBox root_bounds)
		: root_(std::make_unique<node>()), max_depth_(MAX_DEPTH) {
		root_->bounds_ = root_bounds;
		root_->life_ = 0;
		root_->depth_ = 0;
	}
	// creates an octree of the defined depth
	octree(BoundingBox root_bounds, int depth)
		: octree(root_bounds) {
		max_depth_ = depth;
		// build the children until the desired depth is reach
		build_children(root_); // so depth is now 1

		// consider how to recursively generate the tree, would it be a BFS / DFS kind of 
		// implementation, perhaps a stack is necessary
	}
	// creates an empty octree, then populates it with the list of objects
	template<typename InputIt>
	octree(BoundingBox root_bounds, InputIt first, InputIt last)
		: octree(root_bounds) { // initialise the root node
		for (auto i = first; i != last; ++i) {
			insert(*i);
		}
	}

	octree(BoundingBox root_bounds, std::vector<std::unique_ptr<Object>>& objects)
		: octree(root_bounds, objects.begin(), objects.end()) {
	}

	// copy and move overloads
	octree(const octree& other);
	octree(octree&& other);

	octree& operator= (const octree& other);
	octree& operator=(octree&& other);

	void insert(std::unique_ptr<Object>& obj) {
		insert(root_, obj);
	}
	void erase(std::unique_ptr<Object>& obj);
	std::unique_ptr<Object> extract(std::unique_ptr<Object>& obj);
	void reposition(std::unique_ptr<Object>& obj);

	std::unique_ptr<node>& get_root() {
		return root_;
	}
	std::vector<std::unique_ptr<node>>& get_children() {
		return root_->children_;
	}
	std::vector<std::unique_ptr<Object>>& get_objects() {
		return root_->objects_;
	}

	int height() {
		return height(root_);
	}
	int size() {
		return size(root_);
	}

	bool is_leaf() {
		return is_leaf(root_);
	}
	bool is_empty() {
		return is_empty(root_);
	}

	// checks leaves for their life, prunes if need be
	void check_leaves(double delta) {
		check_leaves(root_, delta);
	}

	node* find_object_node(std::unique_ptr<Object>& obj) {
		return find_object_node(root_, obj);
	}

	Object* find_object(std::unique_ptr<Object>& obj) {
		return find_object(root_, obj);
	}

	// for testing purposes 
	bool object_in_node(BoundingBox& node, BoundingBox& obj) {
		return node_contains_object(node, obj);
	}

};
