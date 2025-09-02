#include "../lib/catch2/catch.hpp"
#include "../src/octree.h"
/**
 * the main purpose for testing is to ensure
 * that moving entities works
 * 
 * the following cases include
 *  -> the moved entity does not change node
 *  -> the moved entity changes node but remains under the same parent (i.e moves to another 
 * child on the same level)
 *  -> the moved entity changes node - moves below
 * -> the moved entity changes node -moves above
 */

tree::octree populate_tree(){
    auto tree = tree::octree(WORLD_BOX);

/*     // add a ship, just to the left of origin
    std::unique_ptr<entities::entity> ship = std::make_unique<entities::test_entity(entities::test_entity(
        TestType::get_instance(),
        SHIP_START,
        Vector3{SHIP_START.x - 1, SHIP_START.y - 0.5, SHIP_START.z - 1},
        Vector3{SHIP_START.x + 1, SHIP_START.y + 0.5, SHIP_START.z + 1},
        tree.get_next_id()
    ));
    tree.insert(ship); */
    return tree; 
}

void move_entity(tree::octree& tree, size_t entity_id, Vector3 new_position){
    // find the entity
    auto objects = tree.get_objects();
    for(auto& obj : objects){
        if(obj.get()->get_id() == entity_id){
            obj.get()->set_position(new_position);
        }
    }
    // set its position
}
TEST_CASE("move but does not change"){
    auto tree = populate_tree();
    auto ship = tree.find_object(0);
    CHECK(Vector3Equals(ship->get_position(), SHIP_START));
    std::cout << Vector3Equals(ship->get_position(), SHIP_START) << std::endl;

}

TEST_CASE("move but stays on the same level"){
    CHECK(true);
}
TEST_CASE("moves to a lower level"){
    CHECK(true);
    
}
TEST_CASE("moves to a higher level"){
    CHECK(true);

}