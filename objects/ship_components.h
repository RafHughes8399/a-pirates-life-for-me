#pragma once 
#include <memory>
#include "raylib.h"
#include "raymath.h"

#include "../game/config.h"

class Sail {
public:
	~Sail() = default;
	Sail(Vector3 direction)
		: direction_(direction) {
		length_ = 0.0f;
	};
	Sail(const Sail& other)
		: direction_(other.direction_), length_(other.length_) {
	};
	Sail(const Sail&& other)
		:direction_(std::move(other.direction_)), length_(other.length_) {
	};

	Vector3 get_sail_direction();
	float get_sail_length();

	void move_sail(Vector3 offest);

	void raise_sail(float length);
	void drop_sail(float length);

private:
	Vector3 direction_;
	float length_;
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
		:state_(std::make_unique<RaisedState>(RaisedState(0.0f, Vector3{1.0f, 0.0f, 1.0f}))), depth_(0.0f) {
	};
	Anchor(const Anchor& other)
		: state_(nullptr), depth_(other.depth_) {

		// Need to create a new state object based on the type of other.state_, temp implementation
		if (auto* raised = dynamic_cast<RaisedState*>(other.state_.get())) {
			state_ = std::make_unique<RaisedState>(*raised);
		}
		else if (auto* lowered = dynamic_cast<LoweredState*>(other.state_.get())) {
			state_ = std::make_unique<LoweredState>(*lowered);
		}
		else if (auto* lowering = dynamic_cast<LoweringState*>(other.state_.get())) {
			state_ = std::make_unique<LoweringState>(*lowering);
		}
		else if (auto* raising = dynamic_cast<RaisingState*>(other.state_.get())) {
			state_ = std::make_unique<RaisingState>(*raising);
		}
	};
	Anchor(Anchor&& other)
		: state_(std::move(other.state_)), depth_(other.depth_) {
	}

	void lower();
	void raise();
	void update();
	Vector3 get_force(); // to apply to the ship

private:

	class AnchorState {
	public:
		virtual ~AnchorState() = default;
		AnchorState(float speed, Vector3 force)
			: speed_(speed), force_coefficient_(force) {
		};

		virtual void lower(Anchor* anchor) = 0;
		virtual void raise(Anchor* anchor) = 0;
		float get_speed();
		Vector3 get_force();
	protected:
		float speed_; // changes with the state
		Vector3 force_coefficient_; // that is currently applied to the ship, depends on state
	};
	class RaisedState : public AnchorState {
	public:
		RaisedState(float speed, Vector3 force)
			: AnchorState(speed, force) {};
		void lower(Anchor* anchor) override;
		void raise(Anchor* anchor) override;
	private:

	};
	class LoweredState : public AnchorState {
	public:
		LoweredState(float speed, Vector3 force)
			: AnchorState(speed, force) {
		};
		void lower(Anchor* anchor) override;
		void raise(Anchor* anchor) override;
	private:
	
	};
	class LoweringState : public AnchorState {
	public: 
		LoweringState(float speed, Vector3 force)
			: AnchorState(speed, force) {
		};

		void lower(Anchor* anchor) override;
		void raise(Anchor* anchor) override;
	private:

	};
	class RaisingState : public AnchorState {
	public:
		RaisingState(float speed, Vector3 force)
			: AnchorState(speed, force) {
		};

		void lower(Anchor* anchor) override;
		void raise(Anchor* anchor) override;
	private:

	};
	std::unique_ptr<AnchorState> state_;
	float depth_;
};