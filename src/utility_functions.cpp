#include "utility_functions.h"
#include <cmath>
#include <algorithm>
#include <iostream>
float shortest_distance_unit_circle(float a, float b){
	// try going left, assumes b  >= a
	auto left = std::fmod(b - a, PI2);
	if (left < 0.0) { left += PI2; }

	auto right = std::fmod(a - b, PI2);
	if (right < 0.0) { right += PI2; }
	return std::min(left, right);
}

int get_quadrant(float rad){
	auto pi = PI2 / 2;
	if (0.0 <= rad and rad < pi / 2) {
		return 1;
	}
	else if (pi / 2 <= rad and rad < pi) {
		return 2;
	}
	else if (pi <= rad and rad <= (3 * pi) / 2) {
		return 3;
	}
	else {
		return 4;
	}
}

std::pair<int, int> utility::position_to_chunk(Vector3& pos){
	
	int col = std::abs((WORLD_Z * 0.5) - pos.z);
	int row = std::abs((WORLD_X * -0.5) - pos.x);

	col /= (CHUNK_SIZE);
	row /= (CHUNK_SIZE);

	return std::make_pair(col, row);
}

float utility::get_maximal_variance_axis(Vector3 bb, int axis) {
	switch (axis) {
	case 0:
		return bb.x;
		break;
	case 1:
		return bb.y;
		break;
	case 2:
		return bb.z;
		break;
	default:
		return -1;
		break;
	}
}

// does a contain b, is b in a?
bool utility::contains(BoundingBox a, BoundingBox b){
	return b.min.x >= a.min.x and b.min.y >= a.min.y and b.min.z >= a.max.z
		and b.max.x <= a.max.x and b.max.y <= a.max.y and b.max.z <= a.max.z;
}