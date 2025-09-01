#include "../lib/catch2/catch.hpp"
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
TEST_CASE("move but does not change"){
    CHECK(true);
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