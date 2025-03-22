#pragma once

#include <map>
#include <string>
#include "../objects/object.h"
#include "../game/config.h"
 /**
  * observer pattern between the world and the managers, the world notifies the managers when things
  * change so they can update certain characteristics.
  * 
  * 
  * there is a parent manager class 
  * then sub concrete classes that is tempalted over the type of information
  * that is managed, for example the texture manages a list of textures
  */
/**  manages object interaction in the game */
class Interaction_Manager {
public:
	~Interaction_Manager() = default;
	Interaction_Manager() = default;
	// describes interactions between objects, object interactions exist in pairs, but only one
	//  has a an implmentation

	// so here the ship is interacting with the ocean, 
	void interact(Ship& ship, Ocean& ocean);
	void interact(Ocean& ocean, Ship& ship);
private:

};
class Texture_Manager {

};
/**  manages models within the game  */
class Model_Manager {


};
/**  manages sounds within the game */
class Sound_Manager {

};


class ShaderManager {
public:
	~ShaderManager() = default;
	ShaderManager() = default;
	ShaderManager(const ShaderManager& other)
		: loaded_shaders_(other.loaded_shaders_){
	}
	ShaderManager(ShaderManager&& other)
		: loaded_shaders_(std::move(other.loaded_shaders_)){
	};
	void load_shader(std::pair<const char*, const char*> path);
	void unload_shader(std::pair<const char*, const char*> path);

	Shader get_shader(std::pair<const char*, const char*> path);

private:
	std::map<std::pair<const char*, const char*>, Shader> loaded_shaders_;
};