// pirate-cmake.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <filesystem>

#include "pirate-cmake.h"
#include "game.h"
#include "config.h"
#include "utility_functions.h"

#include "../lib/raylib/src/raylib.h"

void tick(game::game& world);
void render(game::game& world);
void debug(Camera3D& camera, game::game& game);

int main(){
	auto width = GetScreenWidth();
	auto height = GetScreenHeight();
	//SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(width, height, "a pirate life for me");
	// setup the camera

	// temp - will setup some obj factory
	auto player = player::player();
	auto world = environment::world(player);

	auto game = game::game(world, player);
	DisableCursor();
	SetTargetFPS(FPS);
	// adjust the current working directory
	while (!WindowShouldClose()) {
		tick(game);
		render(game);
	}
	CloseWindow();
}

 void tick(game::game& game) {
	game.update();
}

void render(game::game& game) {
	BeginDrawing();
	ClearBackground(WHITE);
	auto camera = game.get_player().get_camera();
	BeginMode3D(camera);

	game.render();

	EndMode3D();

	debug(camera, game);

	EndDrawing();
}

void debug(Camera3D& camera, game::game& game) {

	auto ship = game.get_player().get_ship();
	// FPS Counter
	DrawText(TextFormat("%d", GetFPS()), 40, 40, 30, GREEN);

	// player debug info
	auto text_y = 15;
	DrawRectangle(200, 5, 195, 170, Fade(SKYBLUE, 0.5f));
	DrawRectangleLines(200, 5, 195, 170, BLUE);
	DrawText("ship status: ", 210, text_y, 10, BLACK);
	DrawText(TextFormat("Position: (%06.3f, %06.3f, %06.3f)", ship->get_position().x, ship->get_position().y, ship->get_position().z), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Direction: %06.3f", ship->get_direction()), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Sail Angle: %06.3f", ship->get_sail().get_sail_direction()), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Sail Length: %06.3f", ship->get_sail().get_sail_length()), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Anchor Depth: %06.3f", ship->get_anchor().get_depth()), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Anchor Speed: %06.3f", ship->get_anchor().get_speed()), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Acceleration: (%06.3f, %06.3f, %06.3f)", ship->get_acceleration().x, ship->get_acceleration().y, ship->get_acceleration().z), 210, text_y += 17, 10, BLACK);
	DrawText(TextFormat("Velocity: (%06.3f, %06.3f, %06.3f)", ship->get_velocity().x, ship->get_velocity().y, ship->get_velocity().z), 210, text_y += 17, 10, BLACK);

	auto ship_position = ship->get_position();
	auto chunk = utility::position_to_chunk(ship_position);
	DrawText(TextFormat("Chunk: [%d, %d]",chunk.first, chunk.second), 210, text_y += 17, 10, BLACK);

	// world debug info
	text_y = 15;
	auto world = game.get_world();
	auto wind = ship->get_sail().get_wind();
	DrawRectangle(400, 5, 195, 170, Fade(SKYBLUE, 0.5f));
	DrawRectangleLines(400, 5, 195, 170, BLUE);
	DrawText(TextFormat("Wind Direction %06.3f", wind.x), 410, 30, 10, BLACK);
	DrawText(TextFormat("Wind Speed %06.3f", wind.y), 410, 45, 10, BLACK);
	
	DrawText(TextFormat("Sail Lower %06.3f", ship->get_direction() - (PI /2)), 410, 60, 10, BLACK);
	DrawText(TextFormat("Sail Upper %06.3f", ship->get_direction() + (PI / 2)), 410, 75, 10, BLACK);
	DrawText(TextFormat("Sail  %06.3f", ship->get_sail().get_sail_direction()), 410, 90, 10, BLACK);
	DrawText(TextFormat("Sail Width   %06.3f", ship->get_sail().get_width()), 410, 105, 10, BLACK);

	DrawText(TextFormat("Sail Wind Lower %06.3f", ship->get_sail().get_sail_direction() - ship->get_sail().get_width() / 2), 410, 120, 10, BLACK);
	DrawText(TextFormat("Sail Wind Upper %06.3f", ship->get_sail().get_sail_direction() + ship->get_sail().get_width() / 2), 410, 135, 10, BLACK);




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