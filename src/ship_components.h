#ifndef SHIP_COMPONENTS_H
#define SHIP_COMPONENTS_H

#include <memory>
#include <numbers>
#include <utility>
#include <cmath>
#include "../lib/raylib/src/raylib.h"
#include "../lib/raylib/src/raymath.h"
#include "config.h"
#include "utility_functions.h"

namespace components{
	class sail {
	public:
		~sail() = default;
		sail(float direction, float width)
			: direction_(direction), width_(width), wind_(Vector2Zero()), length_(0.0f), force_(Vector3{1.0f, 1.0f, 1.0f}) {
			calculate_force();
		};
		sail(const sail& other) = default;
		sail(sail&& other) = default;
		
		float get_sail_direction();
		float get_sail_length();
		float get_width();
		Vector3 get_force();
		
		void turn(float delta, float  ship_direction, int turn_direction);
		void move(float length, int direction);
		void raise_sail(float length);
		void lower_sail(float length);
		
		void set_wind(float direction, float speed);
		Vector2& get_wind();
		
		private:
		void calculate_force();
		float direction_; // angle in radians
		float width_;
		// the arc of the sail is r * direction_, r is the length of the sail ig
		float length_;
		Vector2 wind_;
		Vector3 force_;
	};

	class anchor{
	public:

		~anchor() = default;
		anchor() 
			:depth_(0.0f), force_coefficient_(Vector3{ 1.0f, 1.0f, 1.0f }){}
		anchor(const anchor& other) = default;
		anchor(anchor&& other) = default;
			
		void move(float lenght, int direciton);
		Vector3 get_force(); // to apply to the ship
		float get_depth();
	private:
		void calculate_force();
		float depth_;
		Vector3 force_coefficient_; // that is currently applied to the ship, depends on state
	};
}
#endif