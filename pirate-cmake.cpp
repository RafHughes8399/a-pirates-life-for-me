// pirate-cmake.cpp : Defines the entry point for the application.
//

#include "pirate-cmake.h"
#include "environment/world.h"
#include "raylib.h"
#include "game/config.h"

void tick(World& world);
void render(World& world);
void camera_debug(Camera3D& camera);
int main(){
	auto width = GetScreenWidth();
	auto height = GetScreenHeight();
	InitWindow(width, height, "a pirate life for me");
	// setup the camera

	World world = World();
	DisableCursor();
	SetTargetFPS(FPS);
	while (!WindowShouldClose()) {
		tick(world);
		render(world);
	}
	CloseWindow();
}

void tick(World& world) {
	world.update();
}

void render(World& world) {
	BeginDrawing();
	ClearBackground(WHITE);
	auto camera = world.get_player().get_camera();
	BeginMode3D(camera);
	
	world.render();

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