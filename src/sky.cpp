#include "environment.h"

// X is left right
// Z IS front back

// its the x ones that are incorrectly placed 

#define SKY_TOP Vector3{0.0f,  WORLD_Y /2 , 0.0f}
#define SKY_LEFT Vector3 {WORLD_X * -0.5, WORLD_Y/2, 0.0f}
#define SKY_RIGHT Vector3{WORLD_X * 0.5, WORLD_Y/2, 0.0f}
#define SKY_FRONT Vector3{0.0f, WORLD_Y/2,  WORLD_Z * 0.5}
#define SKY_BACK Vector3{0.0f, WORLD_Y/2, WORLD_Z * -0.5}
void environment::sky::generate_sky_faces(){
    // top (WORLD X x WORLD Z)
    
    sky_models_[faces::top] = SkyTopType::get_instance().get_model();
    sky_positions_[faces::top] = SKY_TOP;
    // front (WORLD X x WORLD Y)
    sky_models_[faces::front] =  SkyFrontBack::get_instance().get_model();
    sky_positions_[faces::front] = SKY_FRONT;

    sky_models_[faces::back] =  SkyFrontBack::get_instance().get_model();
    sky_positions_[faces::back] = SKY_BACK;

    // front (WORLD Z x WORLD Y)
    // left
    sky_models_[faces::left] =  SkyLeftRight::get_instance().get_model();
    sky_positions_[faces::left] = SKY_LEFT;

    // front (WORLD Z x WORLD Y)
    // right
    sky_models_[faces::right] = SkyLeftRight::get_instance().get_model();
    sky_positions_[faces::right] = SKY_RIGHT;

}
//TODO: implement
void environment::sky::render(){
    for(size_t i = 0; i < FACES; ++i){
        DrawModel(sky_models_[i], sky_positions_[i], 1.0f, SKY_BLUE);
    }
}