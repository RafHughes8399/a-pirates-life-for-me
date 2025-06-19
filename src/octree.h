
#include <vector>
#include <memory>
#include "object.h"


#define NODE_LIFETIME 30 // how much time a leaf node can exist with
template<typename T>
class octree {
private:
	// node definition
	struct node {
		std::vector<std::unique_ptr<T>> objects_;
		std::vector<std::unique_ptr<node>> children_;
		BoundingBox bounds_;
		short life_; // how long a node has lived without any objects
	};

	// members
	std::unique_ptr<node> root_;


	// methods

	// insert and remove a T from the octree
	void insert(std::unique_ptr<node>& tree, std::unique_ptr<T>& object) {
		return;
	}
	void erase(std::unique_ptr<node>& tree, std::unique_ptr<T>& object);

	void add_node();
	void remove_node();

	// reposition a T within the tree after it moves
	std::unique_ptr<Object> extract(std::unique_ptr<node>& tree, std::unique_ptr<T>& object);
	void reposition(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object);

	int height(std::unique_ptr<node>& tree);
	int size(std::unique_ptr<node>& tree);


	bool is_empty(std::unique_ptr<node>& tree) {
		// check the current list 
		auto empty = tree->objects_.size() == 0 ? true : false;
		// check the children
		if (! empty) { return false; }
		for (auto i = tree->children_.begin(); i != tree->children_.end(); ++i) {
			if (! is_empty(*i)) {
				return false;
			}
		}
		return true;
	}
	bool is_leaf(std::unique_ptr<node>& tree) {
		return tree->children_.size() == 0 ? true : false;
	}

	void prune_tree(std::unique_ptr<node>& tree);

	void check_leaves(std::unique_ptr<node>& tree) {
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

	octree(BoundingBox root_bounds, std::vector<std::unique_ptr<T>>& objects)
		: octree(root_bounds, objects.begin(), objects.end()) {
	}

	// copy and move overloads
	octree(const octree& other);
	octree(octree&& other);

	octree& operator= (const octree& other);
	octree& operator=(octree&& other);

	void insert(std::unique_ptr<T>& obj) {
		insert(root_, obj);
	}
	void erase(std::unique_ptr<T>& obj);
	std::unique_ptr<T> extract(std::unique_ptr<T>& obj);
	void reposition(std::unique_ptr<T>& obj);

	std::unique_ptr<node>& get_root() {
		return root_;
	}
	std::vector<std::unique_ptr<node>>& get_children() {
		return root_->children_;
	}
	std::vector<std::unique_ptr<T>>& get_objects() {
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
	void check_leaves() {
		check_leaves(root_);
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
