#pragma once
#include "world.h"
#include "player.h"
#include "config.h"
// #include "hud.h"

// currently world, player and hud
// events too maybe 


class Game {
public:
	~Game() = default;
	Game(World& world, Player& player)
		: world_(world), player_(player) {


		// here in the game, add event listeners basewd on what you want to listen to
		camera_height_ = 2.0 * RENDER_DISTANCE * tanf(player_.get_camera().fovy * DEG2RAD / 2.0);
		camera_width_ = camera_height_ / (GetScreenWidth() / GetScreenHeight());
	}
	Game(const Game& other)
		: world_(other.world_), player_(other.player_) {
	}
	Game(Game&& other) noexcept
		: world_(other.world_), player_(other.player_) {
	}

	Game& operator=(const Game& other) {
		if (this != &other) {
			world_ = other.world_;
			player_ = other.player_;
		}
		return *this;
	}
	Game& operator=(Game&& other) noexcept {
		if (this != &other) {
			world_ = std::move(other.world_);
			player_ = std::move(other.player_);
		}
		return *this;
	}
	// temp until events
	void update();
	void render();

	World& get_world();

	Player& get_player();



	// and define what happens on those events

private:
	World& world_;
	Player& player_;
	// Hud hud_;


	float camera_height_;
	float camera_width_;
};