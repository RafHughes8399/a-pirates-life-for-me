#ifndef GAME_H
#define GAME_H

// std lib includes 
#include <utility>

// raylib includes 

// project includes
#include "environment.h" // has player, and object, and raylib, and config
#include "config.h"
#include "events_interface.h"
#include "managers.h"
// events too maybe 

namespace game{
	// class save{}
	// class hud{}
	class game {
	public:
		~game() = default;
		game(environment::world& world, player::player& player)
			: world_(world), player_(player), test_event_manager_(managers::event_manager<events::test_event>()) {
			// here in the game, add event listeners basewd on what you want to listen to
		}
		game(const game& other)
			: world_(other.world_), player_(other.player_) {
		}
		game(game&& other) noexcept
			: world_(other.world_), player_(other.player_) {
		}

		game& operator=(const game& other) = default;
		game& operator=(game&& other) noexcept = default;
		// temp until events
		void update();
		void render();

		environment::world& get_world();

		player::player& get_player();



		// and define what happens on those events

	private:
		environment::world& world_;
		player::player& player_;
		managers::event_manager<events::test_event> test_event_manager_;
		// Hud hud_;
	};

class test_game {
	public:
		~test_game() = default;
		test_game(environment::world& world, player::test_player& player)
			: world_(world), player_(player){
			// here in the test_game, add event listeners basewd on what you want to listen to
		}
		test_game(const test_game& other)
			: world_(other.world_), player_(other.player_) {
		}
		test_game(test_game&& other) noexcept
			: world_(other.world_), player_(other.player_) {
		}

		test_game& operator=(const test_game& other) {
			if (this != &other) {
				world_ = other.world_;
				player_ = other.player_;
			}
			return *this;
		}
		test_game& operator=(test_game&& other) noexcept {
			if (this != &other) {
				world_ = std::move(other.world_);
				player_ = std::move(other.player_);
			}
			return *this;
		}
		// temp until events
		void update();
		void render();

		environment::world& get_world();

		player::test_player& get_player();



		// and define what happens on those events

	private:
		environment::world& world_;
		player::test_player& player_;
		// Hud hud_;

	};
}
#endif