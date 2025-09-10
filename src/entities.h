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
#include "controls.h"
#include "events_interface.h"

#define TURN_LEFT 0
#define TURN_RIGHT 1
#define SAIL_UP 2
#define SAIL_DOWN 3 
#define SAIL_LEFT 4
#define SAIL_RIGHT 5
#define ANCHOR_UP 6
#define ANCHOR_DOWN 7


namespace entities{
	enum status_codes{
		nothing = 0,
		moved = 1,
		dead = 2, 
		other = 3,
		//TODO: insert more as needed
	};


	class entity {
		public:
		virtual ~entity() = default;
		entity(ObjectType& object_type, Vector3 position, Vector3 min, Vector3 max, int id)
			: object_type_(object_type),position_(position), bounding_box_(BoundingBox{min, max}), id_(id){
			// generate the bounding box, min and max
		};
	
		entity(const entity& other) = default;
		entity(entity&& other) = default;
		// a default update and render, the default update does nothing, render just draws the 
		virtual int update(float delta);
		virtual void render();
		virtual void interact(entity& other) = 0; // for now just the interaction is 
		virtual std::unique_ptr<entity> clone() = 0; 
		float get_height();
		float get_width();
		float get_length();
		int get_id();
		
		Model& get_model();
		Vector3 get_position();
		BoundingBox& get_bounding_box();
		void update_bounding_box();
		
		void set_position(Vector3 position){
			position_ = position;
		}
		// operator overloads
		friend bool operator==(const entity& a, const entity& b){
			return a.id_ == b.id_;
		}
		protected:
		// all objects have a model and position, and a hitbox, will get to hitboxs
		ObjectType& object_type_;
		Vector3 position_;
		BoundingBox bounding_box_;
		int id_;
};
class static_entity : public entity{
	public:
	~static_entity() = default;
	static_entity(ObjectType& obj_type, Vector3 position, Vector3 min, Vector3 max, int id)
		:entity(obj_type, position, min, max, id){};
	static_entity(const static_entity& other) = default;
	static_entity(static_entity&& other) = default;
	
	void interact(entity& other) override;
	std::unique_ptr<entity> clone() override;
	protected:
	
};
class moveable_entity : public entity {
public:	
~moveable_entity() = default;
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
		int update(float delta) override;
		void render() override;
		void interact(entity& other) override;
		std::unique_ptr<entity> clone() override;
		
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
		
	int update(float delta) override;
	void render() override;
	void interact(entity& other) override;
	std::unique_ptr<entity> clone() override;
	
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


class ocean : public static_entity {
	public:
	~ocean() = default;
	ocean(OceanType& ocean_type, Vector3 position, Vector3 min, Vector3 max, int id)
	: static_entity(ocean_type, position, min, max, id){
	};
	ocean(const ocean& other) = default;
	ocean(ocean&& other) = default;
	void interact(entity& other) override;
	std::unique_ptr<entity> clone() override;
	void render() override;
	private:
};

class terrain : public static_entity {
	public:
	~terrain() = default;
	terrain(ObjectType& terrain_type, Vector3 position, Vector3 min, Vector3 max, int id)
	: static_entity(terrain_type, position, min, max, id) {};	
	terrain(const terrain& other) = default;
	terrain(terrain&& other)= default;
	
	int update(float delta) override;
	void render() override;
	void interact(entity& other) override;
	std::unique_ptr<entity> clone() override;
	private:
	
};

class test_entity : public static_entity{
	public:
	~test_entity() = default;
	test_entity(ObjectType& test_type, Vector3 position, Vector3 min, Vector3 max, int id)
	: static_entity(test_type, position, min, max, id) {
	};	
	test_entity (const test_entity & other) = default;
	test_entity (test_entity && other)= default;
	
	int update(float delta) override;
	void render() override;
	void interact(entity& other) override;
	std::unique_ptr<entity> clone() override;
	private:
};
}
#endif