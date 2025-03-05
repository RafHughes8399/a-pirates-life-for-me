#pragma once
#include "raylib.h"
#define SEA_LEVEL 0.0;
class Ocean {
public:
	// CONSTRUCTORS
	Ocean()
		: density_(1025.0f), depth_(50.0), ocean_model_(LoadModelFromMesh(GenMeshCube(1000.0f, 50.0f, 1000.0f))){};
	
	Ocean(const Ocean& other)
		: density_(other.density_), depth_(other.depth_), ocean_model_(other.ocean_model_) {
	};
	Ocean(const Ocean&& other);

	Ocean& operator=(const Ocean& ohter);
	Ocean& operator=(const Ocean&& other);

	void update();
	void render();
	float get_density();
private:
	Model ocean_model_;
	float density_; // kg/m3, things float 
	float depth_;

};