#pragma once

class Ocean {
public:
	// CONSTRUCTORS
	Ocean();

	Ocean(const Ocean& other);

	Ocean(const Ocean&& other);

	Ocean& operator=(const Ocean& ohter);
	Ocean& operator = (const Ocean&& other);


	float get_density();
private:
	float density_;

};