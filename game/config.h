#pragma once
#include <numbers>
// game properties
#define FPS 60

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
#define WIND_SPEED_MIN 1.0f;
#define WIND_SPEED_MAX 10.0f
#define WIND_CHANGE_TIME 1.0 // the wind randomised at thsi interval


// ship properties
#define SHIP_TURN_SPEED 0.007f // to be updated maybe ?

// sail properties
#define LOWER_RAISE_SPEED 0.025f
#define SAIL_TURN_SPEED 0.1f

// anchor properties
#define ANCHOR_DROP_SPEED 0.35f // how quick the anchor drops in seconds
#define ANCHOR_STATIC_SPEED 0.0f
#define ANCHOR_MAX_DEPTH 32.6f 
#define ANCHOR_RAISED_FORCE {1.0f, 0.0f, 1.0f}
#define ANCHOR_LOWERED_FORCE {0.0f, 0.0f, 0.0f}

