#include "managers.h"

void Interaction_Manager::interact(Ship& ship, Ocean& ocean) {
	auto buoynacy = Vector3{ 0.0f,0.0f,0.0f };
	auto submerged_height = std::abs(0.0f - ship.get_position().y);
	auto p = ocean.get_density();
	auto g = GRAVITY;
	auto v = ship.get_width() * ship.get_length() * submerged_height;
	buoynacy.y += p * g * v * -1;

	ship.adjust_acceleration(buoynacy);
}

void Interaction_Manager::interact(Ocean& ocean, Ship& ship){
	return;
}

void ShaderManager::load_shader(std::pair<const char*, const char*> path) {
	
	// increase the instances of the shader loaded if it exists
	if (loaded_shaders_.contains(path)) {
		Shader shader = loaded_shaders_[path];
	}
	// if it does not exist load it
	else {
		loaded_shaders_[path] = LoadShader(path.first, path.second);
	}
}

void ShaderManager::unload_shader(std::pair<const char*, const char*> path){
	auto shader = loaded_shaders_.find(path);
	
	// remove from instances loaded

	// unload the shader
	UnloadShader(shader->second);
	
	// remove from loaded_shaders
	loaded_shaders_.erase(path);

}

Shader ShaderManager::get_shader(std::pair<const char*, const char*> path){
	return loaded_shaders_[path];
}
