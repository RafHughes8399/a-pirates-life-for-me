#include <numbers>
#include "../lib/raylib/src/raylib.h"

// locations
#define SHIP_START Vector3{5.0f, 0.0f, 5.0f}

// game models and textures
// CWD is build
#define PIRATE_SHIP_MODEL "../models/medium_ship_brown.glb"
#define PIRATE_SHIP_TEXTURE "../textures/pirate-textures/pirate_ship_texture.png"


#define HUB_TERRAIAN_MODEL "../models/hub_terrain.glb"
#define COVE_TERRAIN_MODEL "../models/cove_terrain.glb"
#define LAGOON_TERRAIN_MODEL "../models/lagoon_terrain.glb"
#define BAY_TERRAIN_MODEL "../models/bay_terrain.glb"
#define ISLE_TERRAIN_MODEL "../models/isle_terrain.glb"
#define REEF_TERRAIN_MODEL "../models/reef_terrain.glb"

// game textures 
#define TEXTURE3B "../textures/pirate-textures/Texture_03_B.png"
#define WATER_TEXTURE "../textures/environment-textures/Core/WaterNormals_01.png"
#define SAND_TEXTURE "../textures/environment-textures/Terrain/Sand_Texture_01.png"


// game properties
#define FPS 60
#define ASPECT_RATIO GetScreenWidth() / GetScreenHeight()


// physics constants
#define GRAVITY -0.05f //  units per second
#define WATER_DENISTY 1.02f
#define LAND_DENSITY 9.8f
#define SEA_LEVEL 0.0f
// camera properties
#define CAMERA_START Vector3{0.0f, 5.0f, 0.0f}
#define TARGET_START Vector3{2.5, 2.5, 2.5};
#define CAMERA_MOVE_SPEED 5.4f       // Units per second
#define CAMERA_ROTATION_SPEED 0.03f
#define CAMERA_PAN_SPEED 0.2f
#define CAMERA_ORBITAL_SPEED 0.5f       // Radians per second
#define CAMERA_MIN_LEVEL 0.25f
#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.003f

// rendering properties
#define FOV 90
#define NEAR 1 // start with 0.5, i think some work can be done in calculating the target of the camera
#define FAR 350 // go for 100 units for now,

// colours
#define SKY_BLUE Color{19, 195, 235, 255}
// angles
#define ZERO 0.0f // in radians
#define PI2 2 * std::numbers::pi_v<float>

// world properties
#define WORLD_X 1524.0f
#define WORLD_Y 128.0f
#define WORLD_Z 1524.0f
#define ORIGIN Vector3Zero()


// wind properties
#define NO_WIND 4.2f
#define WIND_SPEED_MIN	8.3f - NO_WIND
#define WIND_SPEED_MAX 12.4f - NO_WIND
#define WIND_CHANGE_TIME 30.0f // the wind randomised at thsi interval


// ship properties
#define SHIP_DENSITY 5.3f
#define SHIP_TURN_SPEED (2 * PI * 0.065)// to be updated maybe ?
#define SHIP_MASS 1.4f

// sail properties
#define LOWER_RAISE_SPEED 0.6f
#define SAIL_TURN_SPEED (2* PI * 0.15)

// anchor properties
#define ANCHOR_MOVE_SPEED 0.75f
#define ANCHOR_STATIC_SPEED 0.0f
#define ANCHOR_MAX_DEPTH 1.5f 
#define ANCHOR_RAISED_FORCE Vector3{1.0f, 0.0f, 1.0f}
#define ANCHOR_LOWERED_FORCE Vector3{0.0f, 0.0f, 0.0f}



// locations 

// sprites
#define ANCHOR_HUD_PATH "../sprites/anchor.png"
#define ANCHOR_HUD_WIDTH_TOTAL 6270
#define ANCHOR_HUD_HEIGHT_TOTAL 190

#define ANCHOR_HUD_FRAMES 57
#define ANCHOR_HUD_ANIMATIONS 1 

#define ANCHOR_HUD_WIDTH_FRAME ANCHOR_HUD_WIDTH_TOTAL / ANCHOR_HUD_FRAMES
#define ANCHOR_HUD_HEIGHT_FRAME ANCHOR_HUD_HEIGHT_TOTAL


#define COMPASS_HUD_PATH "../sprites/compass.png" // TODO 6/10 pending creation of the art, does not exist yet
#define COMPASS_HUD_WIDTH_TOTAL  0 // TODO 6/10 replace with actual value
#define COMPASS_HUD_HEIGHT_TOTAL  0 // TODO 6/10 replace with actual value
#define COMPASS_HUD_FRAMES 90 // compass has 360 degrees, each frame is equivalent to 4 degrees of movement
#define COMPASS_HUD_ANIMATIONS 1

#define COMPASS_HUD_FRAME_WIDTH COMPASS_HUD_WIDTH_TOTAL / COMPASS_HUD_FRAMES
#define COMPASS_HUD_FRAME_HEIGHT COMPASS_HUD_HEIGHT_TOTAL / COMPASS_HUD_ANIMATIONS

#define SAIL_HUD_PATH "../sprites/SAIL.png" // TODO 6/10 pending creation of the art, does not exist yet
#define SAIL_HUD_WIDTH_TOTAL  0 // TODO 6/10 replace with actual value
#define SAIL_HUD_HEIGHT_TOTAL  0 // TODO 6/10 replace with actual value
#define SAIL_HUD_FRAMES 30 // TODO 6/10 replace with actual value 
#define SAIL_HUD_ANIMATIONS 3 // TODO 6/10 replace with actual value

#define SAIL_HUD_FRAME_WIDTH SAIL_HUD_WIDTH_TOTAL / SAIL_HUD_FRAMES
#define SAIL_HUD_FRAME_HEIGHT SAIL_HUD_HEIGHT_TOTAL / SAIL_HUD_ANIMATIONS

// map behaves somewhat differently , there is only one big map but the frame depends on the player position
#define MAP_HUD_PATH "../sprites/minimap.png" // TODO 6/10 pending creation of the art, does not exist yet
#define MAP_HUD_WIDTH_TOTAL  0 // TODO 6/10 replace with actual value
#define MAP_HUD_HEIGHT_TOTAL  0 // TODO 6/10 replace with actual value
#define MAP_HUD_FRAMES 1 
#define MAP_HUD_ANIMATIONS 1

#define MAP_HUD_FRAME_WIDTH MAP_HUD_WIDTH_TOTAL / MAP_HUD_FRAMES
#define MAP_HUD_FRAME_HEIGHT MAP_HUD_HEIGHT_TOTAL / MAP_HUD_ANIMATIONS
