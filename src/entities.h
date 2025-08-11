#ifndef ENTITIES_H
#define ENTITIES_H
#include <utility>
#include <string>
#include <cmath>
#include <iostream>
#include <map>

#include "../lib/raylib/src/raylib.h"
#include "../lib/raylib/src/raymath.h"

#include "singleton_flyweight.h"
#include "ship_components.h"
#include "config.h"
#include "managers.h"
#include "controls.h"

#define TURN_LEFT 0
#define TURN_RIGHT 1
#define SAIL_UP 2
#define SAIL_DOWN 3 
#define SAIL_LEFT 4
#define SAIL_RIGHT 5
#define ANCHOR_UP 6
#define ANCHOR_DOWN 7
namespace entities{

	class entity {
		public:
		virtual ~entity() = default;
		entity(ObjectType& object_type, Vector3 position, Vector3 min, Vector3 max, int id)
			: object_type_(object_type),position_(position), bounding_box_(BoundingBox{min, max}), id_(id){
			// generate the bounding box, min and max
		};
	
		entity(const entity& other)
			: object_type_(other.object_type_), position_(other.position_), bounding_box_(other.bounding_box_), id_(other.id_){
			};
		
		entity(entity&& other)
			: object_type_(other.object_type_), position_(std::move(other.position_)), bounding_box_(std::move(other.bounding_box_)), id_(std::move(other.id_)){
			};
		
		
		// a default update and render, the default update does nothing, render just draws the 
		virtual void update(float delta);
		virtual void render();
		virtual void interact(entity* other);
		float get_height();
		float get_width();
		float get_length();
		
		int get_id();
		
		// getters and setters 
		Model& get_model();
		Vector3 get_position();
		BoundingBox& get_bounding_box();
		void update_bounding_box();
		
		// operator overloads
		friend bool operator==(const entity& a, const entity& b){
			return a.id_ == b.id_;
		}
		protected:
		
		// all objects have a model and position, and a hitbox
		ObjectType& object_type_;
		Vector3 position_;
		BoundingBox bounding_box_;
		int id_;
};

class moveable_entity : public entity {
public:	
	moveable_entity() = default;
	moveable_entity(ObjectType& object_type, Vector3 position, Vector3 min, Vector3 max, int id,  Vector3 velocity, float direction)
		: entity(object_type, position, min, max,  id), velocity_(velocity), direction_(direction) {
			acceleration_ = Vector3{ 0.0,0.0,0.0 };
	};
	moveable_entity(const moveable_entity& other)
		: entity(other), velocity_(other.velocity_), acceleration_(other.acceleration_), direction_(other.direction_){
	};

	moveable_entity(moveable_entity&& other)
		: entity(other), velocity_(std::move(other.velocity_)), acceleration_(std::move(other.acceleration_)), direction_(std::move(other.direction_)) {
	}
	void update(float delta) override;
	void render() override;
	Vector3 get_acceleration();
	Vector3 get_velocity();
	float get_direction();
	Vector3 get_direction_coefficient();
	void adjust_acceleration(Vector3 acceleration);
	
	
	protected:
	Vector3 velocity_;
	Vector3 acceleration_;
	float direction_; // in radians
};

// TODO potential rename to distinguish between player_ships and npc_ships, that is how you differentiate
// yet for a later day
class player_ship : public moveable_entity {
public:
	~player_ship(){
		// unsubscribe
		event_interface::unsubscribe<events::player_input_event>(player_input_handler_);
	}
	// init the handler and subscribe
	player_ship(ShipType& ship_type, Vector3 position, Vector3 min, Vector3 max, int id, Vector3 velocity = Vector3Zero(), float direction = 0.0f)
			: moveable_entity(ship_type, position, min, max, id, velocity, direction),
			 sail_(components::sail(direction, 4.2f)), anchor_(components::anchor()),
			player_input_handler_([this](const events::player_input_event& event){on_player_input_event(event);}){
		
			init_control_map();
			event_interface::subscribe<events::player_input_event>(player_input_handler_);

	};
	
	player_ship(const player_ship& other)
		: moveable_entity(other), sail_(other.sail_), anchor_(other.anchor_), 
		player_input_handler_(other.player_input_handler_), control_map_(other.control_map_){
		};

	player_ship(player_ship&& other)
		: moveable_entity(other), sail_(std::move(other.sail_)), anchor_(std::move(other.anchor_)), 
		player_input_handler_(std::move(other.player_input_handler_)){
	};
	
	player_ship& operator=(const player_ship& ohter);
	player_ship& operator= (const player_ship&& other);
	
	void update(float delta) override;
	void render() override;
	void interact(entity* other) override;
	void set_position(Vector3 position);
	

	Vector3 get_position();
	components::sail get_sail();
	components::anchor get_anchor();

	void on_player_input_event(const events::player_input_event& event);
	void move_anchor();
	void steer_ship(float delta, int direction);
	void move_sail(float delta, int direction);
	void turn_sail(float delta, int turn_direction);
	void move_anchor(float delta, int direction);

	void update_sail_wind(float direction, float speed);

private:
	void init_control_map();
	components::sail sail_;
	components::anchor anchor_;
	events::event_handler<events::player_input_event> player_input_handler_;

	std::map<int, std::function<void(float delta)>> control_map_;
};


class ocean : public entity {
	public:
	ocean(OceanType& ocean_type, Vector3 position, Vector3 min, Vector3 max, int id)
	: entity(ocean_type, position, min, max, id){
	};
	
	ocean(const ocean& other)
	: entity(other){
	};
	
	ocean(ocean&& other)
	: entity(other){
	};
	void interact(entity* other) override;
	void render() override;
	private:
};

class terrain : public entity {
	public:
	terrain(ObjectType& terrain_type, Vector3 position, Vector3 min, Vector3 max, int id)
	: entity(terrain_type, position, min, max, id) {
	};	
	terrain(const terrain& other)
	: entity(other) {
	};
	terrain(terrain&& other)
	: entity(other) {
	};
	
	void update(float delta) override;
	void render() override;
	void interact(entity* other) override;
	private:

};

class test_entity : public entity{
	public:
	test_entity(ObjectType& test_type, Vector3 position, Vector3 min, Vector3 max, int id)
	: entity(test_type, position, min, max, id) {
	};	
	test_entity (const test_entity & other)
	: entity(other) {
	};
	test_entity (test_entity && other)
	: entity(other) {
	};
	
	void update(float delta) override;
	void render() override;
	void interact(entity* other) override;
	private:
};
}
#endif