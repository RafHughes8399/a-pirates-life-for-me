// pirate-cmake.cpp : Defines the entry point for the application.
//

#include "pirate-cmake.h"
#include "environment/world.h"
#include "raylib.h"
#include "game/config.h"

void tick(World& world);
void render(World& world);
void debug(Camera3D& camera, World& world);
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

	debug(camera, world);

	EndDrawing();
}

void debug(Camera3D& camera, World& world) {

	auto ship = *world.get_player().get_ship();
	// FPS Counter
	DrawText(TextFormat("%d", GetFPS()), 40, 40, 30, GREEN);

	// player debug info
	auto text_y = 15;
	DrawRectangle(200, 5, 195, 170, Fade(SKYBLUE, 0.5f));
	DrawRectangleLines(200, 5, 195, 170, BLUE);
	DrawText("Ship status: ", 210, text_y, 10, BLACK);
	DrawText(TextFormat("Position: (%06.3f, %06.3f, %06.3f)", ship.get_position().x, ship.get_position().y, ship.get_position().z), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Direction: %06.3f", ship.get_direction()), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Sail Angle: %06.3f", ship.get_sail().get_sail_direction()), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Sail Length: %06.3f", ship.get_sail().get_sail_length()), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Anchor Depth: %06.3f", ship.get_anchor().get_depth()), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Anchor Speed: %06.3f", ship.get_anchor().get_speed()), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Acceleration: (%06.3f, %06.3f, %06.3f)", ship.get_acceleration().x, ship.get_acceleration().y, ship.get_acceleration().z), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Velocity: (%06.3f, %06.3f, %06.3f)", ship.get_velocity().x, ship.get_velocity().y, ship.get_velocity().z), 210, text_y += 17, 10, BLACK);

	// world debug info
	text_y = 15;
	auto wind = world.get_wind();
	DrawRectangle(400, 5, 195, 170, Fade(SKYBLUE, 0.5f));
	DrawRectangleLines(400, 5, 195, 170, BLUE);
	DrawText(TextFormat("Wind Direction %06.3f", wind.get_direction()), 410, 30, 10, BLACK);
	DrawText(TextFormat("Wind Speed %06.3f", wind.get_speed()), 410, 45, 10, BLACK);

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