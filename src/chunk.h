#pragma once
#include <vector>
#include <memory>
#include "object.h"
#include "raylib.h"

class Chunk {
public:
	~Chunk() = default;
	Chunk(BoundingBox chunk_bounds)
		: chunk_bounds_(chunk_bounds) {
	}
	Chunk(const Chunk& other)
		: chunk_bounds_(other.chunk_bounds_) {
		for (const auto& o : other.objects_) {
			objects_.push_back(o);
		}
	}
	Chunk(Chunk&& other) noexcept
		: chunk_bounds_(std::move(other.chunk_bounds_)) {
	}

	Chunk& operator=(const Chunk& other) {
		if (this != &other) {
			chunk_bounds_ = other.chunk_bounds_;
			for (const auto& o : other.objects_) {
				objects_.push_back(o);
			}
		}
		return *this;
	}

	Chunk& operator=(Chunk&& other) noexcept {
		if (this != &other) {
			chunk_bounds_ = std::move(other.chunk_bounds_);
		}
		return *this;
	}

	void render_chunk();
	void update_chunk();
private:
	std::vector<std::shared_ptr<Object>> objects_;
	BoundingBox chunk_bounds_;
};