// pirate-cmake.cpp : Defines the entry point for the application.
//

#include "pirate-cmake.h"
#include "raylib.h"



// some numbers that need to be set first
// 0.0 y is sea level. these values reflfect metres

static const Vector3 ORIGIN = { 0.0, 0.0, 0.0 };
static const float SEA_LEVEL = 0.0;
void tick(Camera3D & camera);
void render(Camera3D & camera, int camera_mode);
void camera_debug(Camera3D& camera);
int main(){
	auto width = GetScreenWidth();
	auto height = GetScreenHeight();
	InitWindow(width, height, "a pirate life for me");
	// setup the camera
	Camera3D camera = {}; // takes a position, target, an up vector, field of view and projection
	camera.position = Vector3{ 0.0, 5.0, 5.0 };
	camera.target = Vector3{ 0.0,0.5,0.0 }; // the camera looks at the cube, slightly above sea level
	camera.up = Vector3{ 0.0, 1.0, 0.0 }; // rotation toward target
	camera.fovy = 90;
	camera.projection = CAMERA_PERSPECTIVE; // should be third person mode ? 

	DisableCursor();
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		tick(camera);
		auto camera_mode = CAMERA_THIRD_PERSON;
		render(camera, camera_mode);
	}
	CloseWindow();
}

void tick(Camera3D & camera) {
	UpdateCamera(&camera, CAMERA_THIRD_PERSON);
}

void render(Camera3D & camera, int camera_mode) {
	BeginDrawing();
	ClearBackground(WHITE);
	BeginMode3D(camera);
	DrawGrid(100, 1);
	// draw the water
	auto ocean_origin = Vector3{ ORIGIN.x , ORIGIN.y - 5.0f, ORIGIN.z };
	DrawCube(ocean_origin, 100.0f, 10.0f, 100.0f, BLUE);
	// draw the Ship
	DrawCube(camera.target, 1.0, 1.5, 4.5, DARKGREEN);
	EndMode3D();
	camera_debug(camera);
	EndDrawing();
}

void camera_debug(Camera3D& camera) {
	// camera debug info
	DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
	DrawRectangleLines(600, 5, 195, 100, BLUE);
	DrawText("Camera status:", 610, 15, 10, BLACK);
	DrawText(TextFormat("- Projection: %s", (camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
		(camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"), 610, 45, 10, BLACK);
	DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
	DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
	DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);
}