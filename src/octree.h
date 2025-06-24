
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
		short life_; // how long a node has lived without any objects
	};

	// members
	std::unique_ptr<node> root_;
	// methods

	bool node_contains_object(BoundingBox& node, BoundingBox& object) {
		return (object.min.x > node.min.x and object.min.y > node.min.y and object.min.z > node.min.z)
			and (object.max.x < node.max.x and object.max.y < node.max.y and object.max.z < node.max.z);
	}
	// assumes that the current node has no children
	void build_children(std::unique_ptr<node>& tree) {
		std::cout << "build children " << std::endl;
		// first do quadrants then do octant	
		// left or right, bottom or top, front or back
		auto centre_point = Vector3Add(tree->bounds_.max, tree->bounds_.min);
		centre_point = Vector3Scale(centre_point, 0.5);

		// left bottom front, (min -> centre x, y) (centre -> max z)
		auto lbf = std::make_unique<node>();
		lbf->bounds_ = BoundingBox{Vector3{tree->bounds_.min.x, tree->bounds_.min.y, centre_point.z},
			Vector3{centre_point.x, centre_point.y, tree->bounds_.max.z}};
		lbf->life_ = 0;
		tree->children_.push_back(std::move(lbf));

		// left top front (min -> centre x), (centre -> max y, z)
		auto ltf = std::make_unique<node>();
		ltf->bounds_ = BoundingBox{Vector3{tree->bounds_.min.x, centre_point.y, centre_point.z},
			Vector3{centre_point.x, tree->bounds_.max.y, tree->bounds_.max.z}
		};
		ltf->life_ = 0;
		tree->children_.push_back(std::move(ltf));

		// left bottom back (min -> centre x, y, z)
		auto lbb = std::make_unique<node>();
		lbb->bounds_ = BoundingBox{tree->bounds_.min, centre_point};
		lbb->life_ = 0;
		tree->children_.push_back(std::move(lbb));

		// left top back (min -> centre x, z) (centre -> max, y)
		auto ltb = std::make_unique<node>();
		ltb->bounds_ = BoundingBox{Vector3{tree->bounds_.min.x, centre_point.y, tree->bounds_.min.z},
			Vector3{centre_point.x, tree->bounds_.max.y, centre_point.z}
		};
		ltb->life_ = 0;
		tree->children_.push_back(std::move(ltb));

		// right bottom front (min -> centre y) (centre -> max x, z)
		auto rbf = std::make_unique<node>();
		rbf->bounds_ = BoundingBox{Vector3{centre_point.x, tree->bounds_.min.y, centre_point.z}, 
			Vector3{tree->bounds_.max.x, centre_point.y, tree->bounds_.max.z}
		};
		rbf->life_ = 0;
		tree->children_.push_back(std::move(rbf));

		// right top front (centre -> max x, y, z)
		auto rtf = std::make_unique<node>();
		rtf->bounds_ = BoundingBox{centre_point, tree->bounds_.max};
		rtf->life_ = 0;
		tree->children_.push_back(std::move(rtf));

		// right bottom back (min -> centre y, z) (centre -> max, x)
		auto rbb = std::make_unique<node>();
		rbb->bounds_ = BoundingBox{Vector3{centre_point.x, tree->bounds_.min.y, tree->bounds_.min.z}, 
			Vector3{tree->bounds_.max.x, centre_point.y, centre_point.z} };
		rbb->life_ = 0;
		tree->children_.push_back(std::move(rbb));

		// right top back (min -> centre z) ( centre -> max x y)
		auto rtb = std::make_unique<node>();
		rtb->bounds_ = BoundingBox{ Vector3{centre_point.x, centre_point.y, tree->bounds_.min.z},
		Vector3{tree->bounds_.max.x, tree->bounds_.max.y, centre_point.z} };
		rtb->life_ = 0;
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
			// if there are no children, generate the children
			if (is_leaf(tree)) {
				std::cout << "tree has " << tree->children_.size() << "children " << std::endl;
				build_children(tree);
				std::cout << "tree has " << tree->children_.size() << "children " << std::endl;
			}
			// then check the children,  
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

	// reposition a Object within the tree after it moves
	std::unique_ptr<Object> extract(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object) {
		return nullptr;
	}
	void reposition(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object) {
		return;
	}

	int height(std::unique_ptr<node>& tree) {
		return 0;
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
	~octree() = default;
	// creates an empty octree with a root node
	octree(BoundingBox root_bounds)
		: root_(std::make_unique<node>()) {
		root_->bounds_ = root_bounds;
		root_->life_ = 0;
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
};


/**	so what is an octrees
 *	each non-leaf node has 8 children
 * .each node spans a particular 3D area, expressed as a BB
 * each node has a middle point 
 * each of the 8 children of a node
 * each node has a list of objects within the space partition
 * the root node is the entire world
 * 
 * 
 * 
 * the main things, i need 
 */
