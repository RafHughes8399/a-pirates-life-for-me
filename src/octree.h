
#include <vector>
#include <memory>
#include "object.h"
template<typename T, int height>
class octree {
private:
	// node definition
	struct node {
		std::vector<std::unique_ptr<Object>> objects_;
		std::vector<std::unique_ptr<node>> children_;
	};

	// members
	std::unique_ptr<node> root_;


	// methods
	
	// insert and remove a T from the octree
	void insert(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object);
	void erase(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object);

	// reposition a T within the tree after it moves
	std::unique_ptr<Object> extract(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object);
	void reposition(std::unique_ptr<node>& tree, std::unique_ptr<Object>& object);

	
	node* get_node(std::unique_ptr<node>& tree);
	int height(std::unique_ptr<node>& tree);
	int size(std::unique_ptr<node>& tree);

public:
	~octree() = default;
	
	// creates an empty octree, of the specified height
	octree();

	// creates an octree of the specified height and inserts the 
	// objects

	octree(std::vector<std::unique_ptr<Object>>& objects);


	// copy and move overloads
	octree(const octree& other);
	octree(octree&& other);

	octree& opeartor=(const octree& other);
	octree& opeartor=(octree&& other);

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
