#pragma once

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


// anchor properties
#define ANCHOR_DROP_SPEED 2.6f // how quick the anchor drops in seconds
#define ANCHOR_STATIC_SPEED 0.0f
#define ANCHOR_MAX_DEPTH 15.5f 
#define ANCHOR_RAISED_FORCE {1.0f, 0.0f, 1.0f}
#define ANCHOR_LOWERED_FORCE {0.0f, 0.0f, 0.0f}

