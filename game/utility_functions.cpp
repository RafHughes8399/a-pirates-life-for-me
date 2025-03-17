#include "utility_functions.h"
#include <cmath>
#include <algorithm>
#include "config.h"

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
