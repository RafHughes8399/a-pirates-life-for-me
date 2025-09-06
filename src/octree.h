#ifndef OCTREE_H
#define OCTREE_H

// std includes
#include <vector>
#include <memory>
#include <algorithm>
// raylib includes 
#include "../lib/raylib/src/raylib.h"
#include "../lib/raylib/src/raymath.h"

// project includes
#include "entities.h"
#include "utility_functions.h"

#define MAX_DEPTH 7
#define NODE_LIFETIME 30
#define CHILDREN 8

#define WORLD_MIN Vector3Scale(Vector3{WORLD_X, WORLD_Y, WORLD_Z}, -0.5)
#define WORLD_MAX Vector3Scale(Vector3{WORLD_X, WORLD_Y, WORLD_Z}, 0.5)
#define WORLD_BOX BoundingBox{WORLD_MIN, WORLD_MAX}

namespace tree{
    class octree {
    protected:
        // node definition
        struct o_node {
            std::vector<std::unique_ptr<entities::entity>> objects_;
            std::vector<std::unique_ptr<o_node>> children_;
            BoundingBox bounds_;
            int depth_;
            short life_; // how long a o_node has lived without any objects
            std::unique_ptr<o_node>* parent_;
        
            friend bool operator==(const o_node& a, const o_node& b) {
                return Vector3Equals(a.bounds_.min, b.bounds_.min) and 
                Vector3Equals(a.bounds_.max, b.bounds_.max);
            }
        };
    private:
        // members 
        std::unique_ptr<o_node> root_;
        int max_depth_;
        size_t next_id_;
        // methods
        // containment checks
        bool node_contains_object(BoundingBox& node, BoundingBox& object);
        int object_contained_by_child(BoundingBox& node, BoundingBox& object);
        // child node construction
        bool is_child_built(std::unique_ptr<o_node>& tree, std::unique_ptr<o_node>& child);
        void build_child(std::unique_ptr<o_node>& tree, int child_to_build);
        // object insert and erase
        void insert(std::unique_ptr<o_node>& tree, std::unique_ptr<entities::entity>& object);
        void insert(std::unique_ptr<o_node>& tree, std::vector<std::unique_ptr<entities::entity>>& objects);
        void erase(std::unique_ptr<o_node>& tree, size_t object_id);
        std::unique_ptr<entities::entity> extract(std::unique_ptr<o_node>& tree, size_t object_id);
        
        void clear(std::unique_ptr<o_node>& tree);
        // object lookup
        o_node* find_object_node(std::unique_ptr<o_node>& tree, std::unique_ptr<entities::entity>& object);
        entities::entity* find_object(std::unique_ptr<o_node>& tree, int id);
        
        std::vector<std::reference_wrapper<std::unique_ptr<entities::entity>>> get_objects(std::unique_ptr<o_node>& tree);
        
        template<typename UnaryPred>
        std::vector<std::reference_wrapper<std::unique_ptr<entities::entity>>> get_objects(std::unique_ptr<o_node>& tree, UnaryPred p){
            // pass the object to the predicate
            auto predicate_objects = std::vector<std::reference_wrapper<std::unique_ptr<entities::entity>>>{};
            if(not tree){
                return predicate_objects;
            }
            for(auto& obj : tree->objects_){
                if(p(obj)){
                    predicate_objects.push_back(obj);
                }
            }
            for(auto& child : tree->children_){
                auto child_objects = get_objects(child, p);
                for(auto child_object : child_objects){
                    predicate_objects.push_back(child_object.get());
                }
            }
            return predicate_objects;
        }
            
        // height, size,  traversal and copying
        int height(std::unique_ptr<o_node>& tree);
        size_t size(std::unique_ptr<o_node>& tree);
        size_t num_nodes(std::unique_ptr<o_node>& tree);
        void traverse_tree(std::unique_ptr<o_node>& tree);
        std::unique_ptr<o_node> copy_tree(o_node* tree, std::unique_ptr<o_node>* parent);
        // tree characteristics
        bool is_root(std::unique_ptr<o_node>& tree);
        bool is_empty(std::unique_ptr<o_node>& tree);
        bool is_leaf(std::unique_ptr<o_node>& tree);
        
        
        void prune_leaves(std::unique_ptr<o_node>& tree, double delta);
        
        // update and render
        void update(std::unique_ptr<o_node>& tree, float delta);
        void identify_collisions(std::unique_ptr<o_node>& tree, std::vector<entities::entity*> parent_entities);
        void render(std::unique_ptr<o_node>& tree);


        void move_entity(std::unique_ptr<o_node>& tree, std::unique_ptr<entities::entity>& entity);
        std::unique_ptr<o_node>* find_new_parent(std::unique_ptr<o_node>& tree, std::unique_ptr<entities::entity>& entity);
        template<typename UnaryPred>
        void render(std::unique_ptr<o_node>& tree, UnaryPred p){
        
            if(not tree){
                return;
            }
            for(auto & entity : tree->objects_){
                std::cout << " check object " << entity->get_id() << std::endl;
                if(p(entity)){
                    entity->render();
                    std::cout << " render object " << entity->get_id() << std::endl;
                }
            }
            for(auto & child : tree->children_){
                render(child, p);
            }
        }
        public:
        // CONSTRUCTORS
        ~octree() = default;
        // creates an empty octree with a root o_node
        octree(BoundingBox root_bounds, int depth=MAX_DEPTH)
        : root_(std::make_unique<o_node>()), max_depth_(depth), next_id_(0) {
            root_->bounds_ = root_bounds;
            root_->life_ = 0;
            root_->depth_ = 0;
            root_->parent_ = nullptr;
            // build lazily
        }
        // creates an empty octree, then populates it with the list of objects
        template<typename InputIt>
        octree(BoundingBox root_bounds, InputIt first, InputIt last)
        : octree(root_bounds) { // initialise the root o_node
            for (auto i = first; i != last; ++i) {
                insert(*i);
            }
        }
        
        octree(BoundingBox root_bounds, std::vector<std::unique_ptr<entities::entity>>& objects)
        : octree(root_bounds, objects.begin(), objects.end()) {
        }
        
        // copy and move overloads, root, depth and next id
        octree(const octree& other)
        :  max_depth_(other.max_depth_), next_id_(other.next_id_){
            root_ = copy_tree(other.root_.get(), nullptr);
        };

        octree(octree&& other);
        
        octree& operator= (const octree& other);
        octree& operator=(octree&& other);
        
        // insert and erase 
        void insert(std::vector<std::unique_ptr<entities::entity>>& objs){
            insert(root_, objs);
            
        }
        void insert(std::unique_ptr<entities::entity>& obj) {
            insert(root_, obj);
            next_id_ += 1;
        }
        void erase(size_t id){
            erase(root_, id);
        }
        std::unique_ptr<entities::entity> extract(size_t id){
            return extract(root_, id);
        }
        void clear(){
            clear(root_);
        }
        // object lookup
        o_node* find_object_node(std::unique_ptr<entities::entity>& obj) {
            return find_object_node(root_, obj);
        }
        
        entities::entity* find_object(int id) {
            return find_object(root_, id);
        }
        template<typename UnaryPred>
        std::vector<std::reference_wrapper<std::unique_ptr<entities::entity>>> get_objects(UnaryPred p){
            return get_objects(root_, p);
        }
        std::vector<std::reference_wrapper<std::unique_ptr<entities::entity>>> get_objects(){
            return get_objects(root_);
        }
        
        // update and render
        void update(double delta){
            std::cout << "=========UPDATE TREE======================" << std::endl; 
            std::cout << num_nodes(root_) << std::endl;
            update(root_, delta);
            auto parent_objects = std::vector<entities::entity*>{};
            std::cout << "=========CHECK COLLISIONS IN TREE======================" << std::endl; 
            identify_collisions(root_, parent_objects); // start with an empty list
        }

        // render the tree within a certain bounding box, default is the whole tree
        void render(){
            render(root_);
        }
        
        template<typename UnaryPred>
        void render(UnaryPred p){
            render(root_, p);
        }
        std::unique_ptr<o_node>& get_root() {
            return root_;
        }
        // accessors
        std::vector<std::unique_ptr<o_node>>& get_children() {
            return root_->children_;
        }
        size_t get_next_id(){
            return next_id_;
        }
        
        int max_depth(){
            return max_depth_;
        }
        // height and size
        int height() {
            return height(root_);
        }
        size_t size() {
            return size(root_);
        }
        size_t num_nodes(){
            return num_nodes(root_);
        }
        
        // tree properties
        bool is_leaf() {
            return is_leaf(root_);
        }
        bool is_root(){
            return is_root(root_);
        }
        bool is_empty() {
            return is_empty(root_);
        }
        
        // checks leaves for their life, prunes if need be
        void prune_leaves(double delta) {
            prune_leaves(root_, delta);
        }
        
        // for testing purposes 
        bool object_in_node(BoundingBox& node, BoundingBox& obj) {
            return node_contains_object(node, obj);
        }
        
        void traverse_tree(){
            traverse_tree(root_);
        }
    };
}
#endif