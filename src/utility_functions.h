#pragma once
#include "config.h"
#include "raylib.h"
namespace utility {
	float shortest_distance_unit_circle(float a, float b);
	
	int get_quadrant(float rad);
	
	float get_maximal_variance_axis(Vector3 bb, int axis);

	Vector2 position_to_chunk(Vector3& pos);
	template <typename N>
	bool in_bounds(N target, N lower, N upper);
	
	template<typename N>
	inline bool in_bounds(N target, N lower, N upper){
		return lower <= target and target <= upper;
	

	}
}
	