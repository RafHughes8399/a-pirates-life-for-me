#pragma once
// game properties
#include <numbers>
#define FPS 60
#define GLSL 330
// physics constants
#define GRAVITY -9.8f //  units per second
#define WATER_DENISTY 1.026f


// camera properties
#define CAMERA_MOVE_SPEED 5.4f       // Units per second
#define CAMERA_ROTATION_SPEED 0.03f
#define CAMERA_PAN_SPEED 0.2f
#define CAMERA_ORBITAL_SPEED 0.5f       // Radians per second

#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.003f


// angle
#define ZERO 0.0f // in radians
#define PI2 2 * std::numbers::pi_v<float>

// wind properties
#define NO_WIND 1.2f
#define WIND_SPEED_MIN 3.2f
#define WIND_SPEED_MAX 7.4f
#define WIND_CHANGE_TIME 30.0f // the wind randomised at thsi interval


// ship properties
#define SHIP_TURN_SPEED (2 * PI * 0.0004)// to be updated maybe ?

// sail properties
#define LOWER_RAISE_SPEED 0.025f
#define SAIL_TURN_SPEED (2* PI * 0.003)

// anchor properties
#define ANCHOR_DROP_SPEED 0.35f // how quick the anchor drops in seconds
#define ANCHOR_STATIC_SPEED 0.0f
#define ANCHOR_MAX_DEPTH 32.6f 
#define ANCHOR_RAISED_FORCE {1.0f, 0.0f, 1.0f}
#define ANCHOR_LOWERED_FORCE {0.0f, 0.0f, 0.0f}


// shader paths 
#define LIGHTING_VS "resources/shaders/glsl330/lighting.vs"
#define LIGHTING_FS "resources/shaders/glsl330/lighting.fs"

// terrain image path
#define PATH_PREFIX "../../../"

#define TERRAIN_IMAGE "images/heightmap.png"
#define NEW_TERRAIN_IMAGE_COLOUR "images/new_heightmap_colour.png"
#define NEW_TERRAIN_IMAGE_GREY "images/new_heightmap_grey.png"