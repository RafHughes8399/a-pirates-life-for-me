#pragma once
#include <numbers>

// game models and textures
#define PIRATE_SHIP_MODEL "../../../models/medium_ship_brown.glb"
#define PIRATE_SHIP_TEXTURE "../../../textures/pirate-textures/pirate_ship_texture.png"


#define HUB_TERRAIAN_MODEL "../../../models/hub_terrain.glb"
#define COVE_TERRAIN_MODEL "../../../models/cove_terrain.glb"
#define LAGOON_TERRAIN_MODEL "../../../models/lagoon_terrain.glb"
#define BAY_TERRAIN_MODEL "../../../models/bay_terrain.glb"
#define ISLE_TERRAIN_MODEL "../../../models/isle_terrain.glb"
#define REEF_TERRAIN_MODEL "../../../models/reef_terrain.glb"

// game textures 
#define TEXTURE3B "../../../textures/pirate-textures/Texture_03_B.png"
#define WATER_TEXTURE "../../../textures/environment-textures/Core/WaterNormals_01.png"
#define SAND_TEXTURE "../../../textures/environment-textures/Core/Sand_Texture_01.png"


// game properties
#define FPS 60
#define RENDER_DISTANCE 1 // chunk radius
#define SIMULATION_DISTANCE 2 // chunk radius


// physics constants
#define GRAVITY -0.3f //  units per second
#define WATER_DENISTY 1.026f
#define LAND_DENSITY 9.8f

// camera properties
#define CAMERA_MOVE_SPEED 5.4f       // Units per second
#define CAMERA_ROTATION_SPEED 0.03f
#define CAMERA_PAN_SPEED 0.2f
#define CAMERA_ORBITAL_SPEED 0.5f       // Radians per second
#define CAMERA_MIN_LEVEL 0.25f
#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.003f


// angles
#define ZERO 0.0f // in radians
#define PI2 2 * std::numbers::pi_v<float>

// world properties
#define WORLD_X 1524.0f
#define WORLD_Y 128.0f
#define WORLD_Z 1524.0f

#define NUM_CHUNKS 25
#define CHUNK_SIZE WORLD_X / NUM_CHUNKS

// wind properties
#define NO_WIND 4.2f
#define WIND_SPEED_MIN	8.3f - NO_WIND
#define WIND_SPEED_MAX 12.4f - NO_WIND
#define WIND_CHANGE_TIME 30.0f // the wind randomised at thsi interval


// ship properties
#define SHIP_DENSITY 5.3f
#define SHIP_TURN_SPEED (2 * PI * 0.065)// to be updated maybe ?

// sail properties
#define LOWER_RAISE_SPEED 0.6f
#define SAIL_TURN_SPEED (2* PI * 0.15)

// anchor properties
#define ANCHOR_DROP_SPEED 0.75f // how quick the anchor drops in seconds
#define ANCHOR_STATIC_SPEED 0.0f
#define ANCHOR_MAX_DEPTH 32.6f 
#define ANCHOR_RAISED_FORCE {1.0f, 0.0f, 1.0f}
#define ANCHOR_LOWERED_FORCE {0.0f, 0.0f, 0.0f}




// terrain image path
#define TERRAIN_IMAGE "../../../images/heightmap.png"