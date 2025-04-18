#pragma once 
#include <memory>
#include <numbers>
#include <utility>
#include <cmath>
#include "raylib.h"
#include "raymath.h"
#include "config.h"

class Sail {
public:
	~Sail() = default;
	Sail(float direction, float width)
		: direction_(direction), width_(width), wind_(Vector2Zero()), length_(0.0f), force_(Vector3{1.0f, 1.0f, 1.0f}) {
		calculate_force();
	};
	Sail(const Sail& other)
		: direction_(other.direction_), length_(other.length_), width_(other.width_), 
		wind_(other.wind_), force_(other.force_) {
		calculate_force();
	};
	Sail(const Sail&& other)
		:direction_(std::move(other.direction_)), length_(std::move(other.length_)), width_(std::move(other.width_)),
		wind_(std::move(other.wind_)) {
		calculate_force();
	};

	float get_sail_direction();
	float get_sail_length();
	float get_width();
	Vector3 get_force();

	void sail_left(float& ship_direction, float delta);
	void sail_right(float& ship_direction, float delta);

	void move_sail_left(float rad);
	void move_sail_right(float rad);

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

class Anchor {
public:

	~Anchor() = default;
	Anchor()
		:state_(std::make_shared<StationaryState>(StationaryState(0.0f))), depth_(0.0f), force_coefficient_(Vector3{ 1.0f, 1.0f, 1.0f }){
	};
	Anchor(const Anchor& other)
		: state_(nullptr), depth_(other.depth_), force_coefficient_(other.force_coefficient_) {

		// Need to create a new state object based on the type of other.state_, temp implementation
		if (auto* raised = dynamic_cast<StationaryState*>(other.state_.get())) {
			state_ = std::make_shared<StationaryState>(*raised);
		}
		else if (auto* lowered = dynamic_cast<MovingState*>(other.state_.get())) {
			state_ = std::make_shared<MovingState>(*lowered);
		}
	};
	Anchor(Anchor&& other)
		: state_(std::move(other.state_)), depth_(other.depth_), force_coefficient_(std::move(other.force_coefficient_)) {
	};

	void move();
	void update();
	Vector3 get_force(); // to apply to the ship
	float get_depth();
	float get_speed();
private:

	class AnchorState {
	public:
		virtual ~AnchorState() = default;
		AnchorState(float speed)
			: speed_(speed){
		};
		virtual void move(Anchor* anchor) = 0;
		float get_speed();
	protected:
		float speed_; // changes with the state
	};
	class StationaryState : public AnchorState {
	public:
		StationaryState(float speed)
			: AnchorState(speed) {};
		virtual void move(Anchor* anchor) override;
	private:

	};
	class MovingState : public AnchorState {
	public:
		MovingState(float speed)
			: AnchorState(speed) {
		};
		virtual void move(Anchor* anchor) override;
	private:
	
	};
		
	void calculate_force();
	std::shared_ptr<AnchorState> state_;
	float depth_;
	Vector3 force_coefficient_; // that is currently applied to the ship, depends on state
};