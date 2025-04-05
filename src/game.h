#pragma once
#include "world.h"
#include "player.h"
// #include "hud.h"

// currently world, player and hud
// events too maybe 


class Game {
public:
	~Game() = default;
	Game(World& world, Player& player)
		: world_(world), player_(player) {
	}
	Game(const Game& other)
		: world_(other.world_), player_(other.player_) {
	}
	Game(Game&& other) noexcept
		: world_(std::move(other.world_)), player_(std::move(other.player_)) {
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
private:
	World world_;
	Player player_;
	// Hud hud_;

};