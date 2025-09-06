#include "octree.h"

// containment checks
bool tree::octree::node_contains_object(BoundingBox& node, BoundingBox& object){
    // compare the bounding box of the node and the object
    return (object.min.x >= node.min.x and object.min.y >= node.min.y and object.min.z >= node.min.z)
    and (object.max.x <= node.max.x and object.max.y <= node.max.y and object.max.z <= node.max.z);
}
// return the child "index" that the object can fit into, if -1 then no child can fit the object
int tree::octree::object_contained_by_child(BoundingBox& node, BoundingBox& object){
    // check if the object will fit into potential children of the node 
    auto centre = Vector3Add(node.max, node.min);
    centre = Vector3Scale(centre, 0.5f);

    // first check if the object crosses the centre of any axis, if it does then no child will fit it
    bool crosses_centre = (object.min.x < centre.x and centre.x < object.max.x) 
    or (object.min.y < centre.y and centre.y < object.max.y) 
    or (object.min.z < centre.z and centre.z < object.max.z);
    
    if(crosses_centre) {return -1;}

    // setup the bounding boxes for the potential children
    auto children = std::vector<BoundingBox>{};
    // 0
    children.push_back(
        BoundingBox{node.min, centre}
    );
    // 1
    children.push_back(
        BoundingBox{Vector3{node.min.x,node.min.y, centre.z},  Vector3{centre.x, centre.y, node.max.z}}
    );
    // 2
    children.push_back(
        BoundingBox{Vector3{node.min.x, centre.y, node.min.z},  Vector3{centre.x, node.max.y, centre.z}}
    );
    // 3
    children.push_back(
        BoundingBox{Vector3{node.min.x, centre.y, centre.z},  Vector3{centre.x, node.max.y, node.max.z}}
    );
    // 4
    children.push_back(
        BoundingBox{Vector3{centre.x, node.min.y, node.min.z},  Vector3{node.max.x, centre.y, centre.z}}
    );
    //5
    children.push_back(
        BoundingBox{Vector3{centre.x, node.min.y, centre.z},  Vector3{node.max.x, centre.y, node.max.z}}
    );
    //6
    children.push_back(
        BoundingBox{Vector3{centre.x, centre.y, node.min.z},  Vector3{node.max.x, node.max.y, centre.z}}
    );
    //7
    children.push_back(
        BoundingBox{centre, node.max}
    );

    // otherwise, check which child will fit the object
    for(size_t i = 0; i < CHILDREN; ++i){
        // check the child that will contain the object
        auto child_node = children.at(i);
        if(node_contains_object(child_node, object)){
            // return the "index" of the child that will fit the node
            return int(i);
        }
    }
    return -1;
}
// child construction 
bool tree::octree::is_child_built(std::unique_ptr<o_node>& tree, std::unique_ptr<o_node>& child){
    for(auto& c : tree->children_){
        if(*c == *child){return true;}
    }
    return false;
}

void tree::octree::build_child(std::unique_ptr<o_node>& tree, int child_to_build){
    auto centre = Vector3Add(tree->bounds_.max, tree->bounds_.min);
    centre = Vector3Scale(centre, 0.5f);

    // create the child
    auto child = std::make_unique<o_node>();
    child->depth_ = tree->depth_ + 1;
    child->life_ = 0;
    child->parent_ = &tree;

    // based on the "index" order used ni the method above to select the appropraite bounding box for the child
    switch (child_to_build)
    {
    case 0:
        child->bounds_ =  BoundingBox{tree->bounds_.min, centre};
        break;
    case 1:
        child->bounds_ = BoundingBox{Vector3{tree->bounds_.min.x,tree->bounds_.min.y, centre.z},  Vector3{centre.x, centre.y, tree->bounds_.max.z}};
        break;
    case 2:
        child->bounds_ = BoundingBox{Vector3{tree->bounds_.min.x, centre.y, tree->bounds_.min.z},  Vector3{centre.x, tree->bounds_.max.y, centre.z}};
        break;
    case 3:
       child->bounds_ = BoundingBox{Vector3{tree->bounds_.min.x, centre.y, centre.z},  Vector3{centre.x, tree->bounds_.max.y, tree->bounds_.max.z}};
        break;
    case 4:
        child->bounds_ = BoundingBox{Vector3{centre.x, tree->bounds_.min.y, tree->bounds_.min.z},  Vector3{tree->bounds_.max.x, centre.y, centre.z}};
        break;
    case 5:
        child->bounds_ = BoundingBox{Vector3{centre.x, tree->bounds_.min.y, centre.z},  Vector3{tree->bounds_.max.x, centre.y, tree->bounds_.max.z}};
        break;
    case 6:
        child->bounds_ = BoundingBox{Vector3{centre.x, centre.y, tree->bounds_.min.z},  Vector3{tree->bounds_.max.x, tree->bounds_.max.y, centre.z}};
        break;
    case 7:
        child->bounds_ = BoundingBox{centre, tree->bounds_.max};
        break;
    default:
        break;
    }

    // if the child has not been built, build it
    if(not is_child_built(tree, child)){
        tree->children_.push_back(std::move(child));
    }
}

// insertion
void tree::octree::insert(std::unique_ptr<o_node>& tree, std::unique_ptr<entities::entity>& object){
	auto object_bounds = object->get_bounding_box();
    // check if the node contains the object, if not then immediately return
	if(not node_contains_object(tree->bounds_, object_bounds)){ return; }
	
    else{
        // if at the max depth then insert, no further children can be constructed
        if(tree->depth_ == max_depth_){
			tree->objects_.push_back(std::move(object));
			return;
		}

        // if not all children for the tree have been built
		else if(tree->children_.size() < CHILDREN){
			// check which child needs to be built, and build that child
            auto child_to_build = object_contained_by_child(tree->bounds_, object_bounds);
            if(child_to_build != -1){
                    build_child(tree, child_to_build);
            }
			else{
				// if no child need be built, then insert into the node
				tree->objects_.push_back(std::move(object));
                return;
			}
		}
        // recursively iterate through the children
	    for (auto& child : tree->children_) {
		// if does fit in a child, recursively insert
            if (node_contains_object(child->bounds_, object_bounds)) {
                insert(child, object);
                return;
            }
		}
        // if this point is reached, there are no children that the object can fit into so insert into the node
        tree->objects_.push_back(std::move(object));
	}
}

void tree::octree::insert(std::unique_ptr<o_node>& tree, std::vector<std::unique_ptr<entities::entity>>& objects){
    // quite similar logic to before, just goes with a list of objects instead, level by level, less overall
    // traversal cost than inserting one by one
    //TODO: implement
    (void) tree;
    (void) objects;
    return;

}

void tree::octree::erase(std::unique_ptr<o_node>& tree, size_t object_id){
    if(not tree){
        return;
    } 
    auto new_end = std::remove_if(tree->objects_.begin(), tree->objects_.end(),
        [object_id](auto& obj) -> bool{
            if(object_id == obj->get_id()){
                return true;
            }
            return false;
        });
    // if nothing is to be erased, then check the children
    if(new_end == tree->objects_.end()){
        for(auto& child : tree->children_){
            erase(child, object_id);
        }

    }
    // an object is to be removed
    else{
        tree->objects_.erase(new_end, tree->objects_.end());
        return;
    }
}

std::unique_ptr<entities::entity> tree::octree::extract(std::unique_ptr<o_node>& tree, size_t object_id){
    if(not tree){return nullptr;}
    // find them remove
    auto entity = std::find_if(tree->objects_.begin(), tree->objects_.end(),
        [object_id](auto& obj) -> bool{
            if(object_id == obj->get_id()){
                return true;
            }
            return false;
        });
    // not in this node - keep looking
    if(entity != tree->objects_.end()){
        auto extracted = std::move(*entity);
        tree->objects_.erase(entity);
        return extracted;
    }
    // otherwise not in this node - keep looking 
    else{
        for(auto& child : tree->children_){
            extract(child, object_id);
        }
    }
}
void tree::octree::clear(std::unique_ptr<o_node>& tree){
    tree->objects_.clear();
    for(auto& child : tree->children_){
        clear(child);
    }
}

// object lookup

tree::octree::o_node* tree::octree::find_object_node(std::unique_ptr<o_node>& tree, std::unique_ptr<entities::entity>& object) {
    if (!tree) {
        return nullptr;
    }
    // iterate through the objects in the node, if equal, return a pointer to the node
    for (auto& obj : tree->objects_) {
        if (*obj == *object) {
            return tree.get();
        }
    }
    // recurse through the children of the node
    for (auto& child : tree->children_) {
        auto result =  find_object_node(child, object);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

entities::entity* tree::octree::find_object(std::unique_ptr<o_node>& tree, int id) {
    if (!tree) return nullptr;

    // Check if object is in current o_node
    for(auto& object : tree->objects_){
        if(object->get_id() == id){
            return object.get();
        }
    }
    // Recursively search children
    for (auto& child : tree->children_) {
        auto result = find_object(child, id);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;  // Not found
}

std::vector<std::reference_wrapper<std::unique_ptr<entities::entity>>> tree::octree::get_objects(std::unique_ptr<o_node>& tree){
    return get_objects(tree, [](auto& object) -> bool{
        (void) object;
        return true;
    });
}
int tree::octree::height(std::unique_ptr<o_node>& tree) {
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
size_t tree::octree::size(std::unique_ptr<o_node>& tree) {
    auto empty = is_empty(tree);
    if (empty) { return 0; }
    if(not tree){
        return 0;
    }
    else {
        auto t_size = tree->objects_.size();
        for (auto& child : tree->children_) {
            t_size += size(child);
        }
        return t_size;
    }
    return 0;
}

size_t tree::octree::num_nodes(std::unique_ptr<o_node>& tree){
    if(tree){
        size_t size = 1;
        for(auto& child : tree->children_){
            size += num_nodes(child);
        }
        return size;
    }
    else{
        return 0;
    }
}

bool tree::octree::is_empty(std::unique_ptr<o_node>& tree) {
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
bool tree::octree::is_root(std::unique_ptr<o_node>& tree){
    return tree->depth_ == 0  ? true : false;
}
bool tree::octree::is_leaf(std::unique_ptr<o_node>& tree) {

    return tree->children_.size() == 0 ? true : false;
}


// i suspect some issues with this, i think it is resetting the pointer 
// but then the parent is now holding onto a null pointer

// i need to remove it from the parent's children list
void tree::octree::prune_leaves(std::unique_ptr<o_node>& tree, double delta) {
    // you're thinking about it wrong i think 
        if (is_leaf(tree) and not is_root(tree) 
            and is_empty(tree)) {
            tree->life_ += short(delta);
            if (tree->life_ >= NODE_LIFETIME) {
                tree.reset(); // but not removed from the 
                return;
            }
        }
        else {
            // if not a leaf node, or is not empty, reset the life
            tree->life_ = 0;
            for (auto& child : tree->children_) {
                prune_leaves(child, delta);
            }
            
            // cull the null children
            auto new_end = std::remove_if(tree->children_.begin(), tree->children_.end(), 
            [](auto& child) -> bool {
                return not child;
            });
            tree->children_.erase(new_end, tree->children_.end());
        }
    return;
} 

std::unique_ptr<tree::octree::o_node> tree::octree::copy_tree(o_node* tree, std::unique_ptr<o_node>* parent){
    if(not tree){
        return nullptr;
    }
    auto copy = std::make_unique<o_node>();
    
    // copy the bounds, life, depth
    copy->bounds_ = tree->bounds_;
    copy->depth_ = tree->depth_;
    copy->life_ = tree->life_;
    copy->parent_ = parent;
    // and then the objects, deep copy 
    for(auto & obj : tree->objects_){
        copy->objects_.push_back(std::make_unique<entities::entity>(*obj));
    }
    auto new_parent = &copy;
    for(auto & child : tree->children_){
        copy->children_.push_back(copy_tree(child.get(), new_parent));
    }
    return copy;
}
void tree::octree::traverse_tree(std::unique_ptr<o_node>& tree){
		// print the box of the node 
		if(!tree){
			return;
		}
		std::cout << "-------NODE-------- \nbounds: "; 
		std::cout << "objects:  " << std::endl; 
		for(auto& object : tree->objects_){
		}
		for(auto& child : tree->children_){
			traverse_tree(child);
		}
		// print its children
		return;
}



void tree::octree::update(std::unique_ptr<o_node>& tree, float delta){
    if(not tree) {return;}

    for(auto& object : tree->objects_){
        auto update = object->update(delta);
        switch (update){
            case entities::status_codes::moved:
                // if moved then append to moved objects
                // check if moved out of the current node
                std::cout << "=========== MOVE ENTITY=============" << std::endl;
                move_entity(tree, object); // moves the entity 
                std::cout << "=========== END MOVE ENTITY=============" << std::endl;
                // my thoughts is here to do somthing along the lines of 
                //check_collisions(object)
                break;
            case entities::status_codes::dead:
                // TODO pending some form of combat implementation

                // something along the lines of 
                //erase(tree, object->get_id());
                break;
            default:
                break;
        }
    }
    //TODO: move objects - refer to notes
    // reinsert moved
    for(auto & child : tree->children_){
        update(child, delta);
    }
    return;
}
void tree::octree::identify_collisions(std::unique_ptr<o_node>& tree , std::vector<entities::entity*> parent_entities){
    if(not tree) {return;}
    // check for collisions with objects from parent nodes
    // basic debugging for tests
    std::cout << "parent entities: " << parent_entities.size() << std::endl;
    std::cout << "compare parent and current entities " << std::endl;
    for(auto& parent_entity : parent_entities){
        for(auto& entity : tree->objects_){
            // check for collisions between parent entity and entity
            if(CheckCollisionBoxes(parent_entity->get_bounding_box(), entity->get_bounding_box())){
                // interact
                std::cout << "interaction between " << parent_entity->get_id() << " and " << entity->get_id() << std::endl;
                parent_entity->interact(*entity);             
            }
        }
    }
    // check within the node, avoid duplicate checks and self checks 
    // so if object 1 is checked against 2 ,
    // then it avoids checking object two against 1, and so on
    std::cout << "check within node, there are  " << tree->objects_.size() << " entities  "<< std::endl;
    if(tree->objects_.size() > 1 ){
        for(auto i = 0; i < tree->objects_.size() - 1; ++i){
            for(auto j = i + 1; j < tree->objects_.size(); ++j){
                std::cout << "compare " << i << " and " << j << std::endl;
                if(CheckCollisionBoxes(tree->objects_[i]->get_bounding_box(), tree->objects_[j]->get_bounding_box())){
                    // interact
                    std::cout << "interaction between " << tree->objects_[i]->get_id() << " and " << tree->objects_[j]->get_id() << std::endl;
                    tree->objects_[i]->interact(*tree->objects_[j]);
                }            
            }
        }
    }
    // then append this node into parent objects and pass to children
    std::for_each(tree->objects_.begin(), tree->objects_.end(), 
    [&parent_entities] (auto& entity) -> void {
        parent_entities.push_back(entity.get());
    });
    std::cout << "parent entities: " << parent_entities.size() <<  std::endl;

    // and recurse through the children
    std::for_each(tree->children_.begin(), tree->children_.end(), 
    [&parent_entities, this] (auto & child){
        identify_collisions(child, parent_entities);
    });
}
void tree::octree::render(std::unique_ptr<o_node>& tree){
    // if null tree skip 
    if(not tree){
        return; 
    }
    // if tree in bounds, render objects 
    for(auto & object : tree->objects_){
            object->render();
    }
    // then iterate through children, only render those in bounds
    for(auto & child : tree->children_){
        render(child);
    }
}

void tree::octree::move_entity(std::unique_ptr<o_node>& tree, std::unique_ptr<entities::entity>& entity){
    // check the bounding box

    // if not still
    if(not node_contains_object(tree->bounds_, entity->get_bounding_box())){
        // extract 
        std::cout << "============EXTRACT==============" << std::endl;
        auto extracted_entity = extract(tree, entity->get_id());
        // find the appropraite parent
        std::cout << "============GET NEW PARENT==============" << std::endl;
        auto new_parent = find_new_parent(tree, extracted_entity);
        // and reinsert at the parent
        std::cout << "============REINSERT==============" << std::endl;
        std::cout << size()  << std::endl;
        insert(*new_parent, extracted_entity);
        std::cout << size()  << std::endl;
        std::cout << "============END REINSERT==============" << std::endl;
    }
    else{
        // do nothing, it does not need to move node 
        std::cout << "still in the same node " << std::endl;
    }
    return;
}

std::unique_ptr<tree::octree::o_node>* tree::octree::find_new_parent(std::unique_ptr<o_node>& tree, std::unique_ptr<entities::entity>& entity){
    auto current = &tree;
    std::cout << "new parent is (" << (*current)->bounds_.min.x << ", " << 
    (*current)->bounds_.min.y << ", " << (*current)->bounds_.min.z << ") (" <<
    (*current)->bounds_.max.x << ", " << (*current)->bounds_.max.y << ", " 
    << (*current)->bounds_.max.z << std::endl; 

    auto entity_bounds = entity->get_bounding_box();
    while(not node_contains_object((*current)->bounds_, entity_bounds)){
        current = (*current)->parent_;
    }
    std::cout << "new parent is (" << (*current)->bounds_.min.x << ", " << 
    (*current)->bounds_.min.y << ", " << (*current)->bounds_.min.z << ") (" <<
    (*current)->bounds_.max.x << ", " << (*current)->bounds_.max.y << ", " 
    << (*current)->bounds_.max.z << std::endl; 
    return current;
}