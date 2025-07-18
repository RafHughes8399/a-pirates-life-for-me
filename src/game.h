
// std lib includes 
#include <utility>

// raylib includes 

// project includes
#include "environment.h" // has player, and object, and raylib, and config
#include "config.h"

// currently world, player and hud
// events too maybe 

namespace game{
	// class save{}
	// class hud{}
	class game {
	public:
		~game() = default;
		game(environment::world& world, player::player& player)
			: world_(world), player_(player) {


			// here in the game, add event listeners basewd on what you want to listen to
			camera_height_ = 2.0 * RENDER_DISTANCE * tanf(player_.get_camera().fovy * DEG2RAD / 2.0);
			camera_width_ = camera_height_ / (GetScreenWidth() / GetScreenHeight());
		}
		game(const game& other)
			: world_(other.world_), player_(other.player_) {
		}
		game(game&& other) noexcept
			: world_(other.world_), player_(other.player_) {
		}

		game& operator=(const game& other) {
			if (this != &other) {
				world_ = other.world_;
				player_ = other.player_;
			}
			return *this;
		}
		game& operator=(game&& other) noexcept {
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

		player::player& get_player();



		// and define what happens on those events

	private:
		environment::world& world_;
		player::player& player_;
		// Hud hud_;


		float camera_height_;
		float camera_width_;
	};
}