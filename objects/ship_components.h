#pragma once 
#include <memory>
#include <numbers>
#include <utility>
#include <cmath>
#include "raylib.h"
#include "raymath.h"
#include "../game/config.h"
#include "../environment/wind.h"

class Sail {
public:
	~Sail() = default;
	Sail(float direction, float width, Wind* wind)
		: direction_(direction), wind_(wind) {
		length_ = 0.0f;
	};
	Sail(const Sail& other)
		: direction_(other.direction_), length_(other.length_), width_(other.width_), wind_(other.wind_) {
	};
	Sail(const Sail&& other)
		:direction_(std::move(other.direction_)), length_(std::move(other.length_)), width_(std::move(other.width_)),
		wind_(std::move(other.wind_)){
	};

	float get_sail_direction();
	float get_sail_length();
	float get_width();
	Vector3 get_force();
	const Wind* get_wind();


	float sail_arc();
	void sail_left();
	void sail_right();

	void raise_sail(float length);
	void lower_sail(float length);

	void set_wind(const Wind* wind);

	void update();

private:
	void calculate_force();
	float direction_; // angle in radians
	float width_;
	// the arc of the sail is r * direction_, r is the length of the sail ig
	float length_;
	const Wind* wind_;
	Vector3 force_;
};

/**
 * basic logic is that the state of the anchor changes the force that it applies to the ship's vertical movement
 * on the x and z axis. when the ship updates, it takes the anchor's current force and applies it to 
 * 
 * when the anchor is in dropped state the ship cannot move
 * 
 * the anchor force can be thought of as coefficients that affect the ship's current acceleration
 * 
 * so (1, 0, 1) means the anchor does not affect the ship
 * so (0.5, 0, 0.5) means the anchor halves the ship's acceleration 
 * and (0, 0, 0) means the ship does not move
 * 
 * 
 * an anchor has a height, the speed of the anchor is how much it changes each frame 
 * if lowered that height increases, there is a max anchor height
 * if raised the height decreases
 * 
 * you have to ask how will this integrate with the ship ?
 * the anchor updates each frame and that is what changes the position
 * raise and lower are only called when the button press happen 
 * 
 * update updates the anchor position by the speed, then the anchor force is applied to the ship
 */
class Anchor {
public:

	~Anchor() = default;
	Anchor()
		:state_(std::make_unique<StationaryState>(StationaryState(0.0f))), depth_(0.0f), force_coefficient_(Vector3{ 1.0f, 0.0f, 1.0f }){
	};
	Anchor(const Anchor& other)
		: state_(nullptr), depth_(other.depth_) {

		// Need to create a new state object based on the type of other.state_, temp implementation
		if (auto* raised = dynamic_cast<StationaryState*>(other.state_.get())) {
			state_ = std::make_unique<StationaryState>(*raised);
		}
		else if (auto* lowered = dynamic_cast<MovingState*>(other.state_.get())) {
			state_ = std::make_unique<MovingState>(*lowered);
		}
	};
	Anchor(Anchor&& other)
		: state_(std::move(other.state_)), depth_(other.depth_) {
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
	std::unique_ptr<AnchorState> state_;
	float depth_;
	Vector3 force_coefficient_; // that is currently applied to the ship, depends on state
};