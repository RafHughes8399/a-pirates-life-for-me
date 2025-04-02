#pragma once
#include <vector>
#include <memory>
#include "object.h"
#include "raylib.h"

// a functor is a class with an overloaded () operator that means the class can be called like a function

class AABBComparator {
	friend class Chunk;
public:
	bool operator()(const std::unique_ptr<Object>& a, const std::unique_ptr<Object>& b) {
		// based on the axis
		auto a_bb = GetModelBoundingBox(a->get_model());
		auto b_bb = GetModelBoundingBox(b->get_model());
		switch (axis_) {
		case 0:
			return a_bb.max.x <= b_bb.min.x;
			break;
		case 1:
			return a_bb.max.y <= b_bb.min.y;
			break;
		case 2:
			return a_bb.max.z <= b_bb.min.y;
			break;
		default:
			return true;
			break;
		}
	}
	int axis_ = 0;
	static  AABBComparator& get_instance() {
		static  AABBComparator instance;
		return instance;
	}
};

class Chunk {
public:
	~Chunk() = default;
	Chunk(Vector3 min, Vector3 max)
		: chunk_bounds_(BoundingBox{ min, max }) {
	}
	Chunk(const Chunk& other)
		: chunk_bounds_(other.chunk_bounds_) {
		for (const auto& o : other.objects_) {
			objects_.push_back(o->clone());
		}
	}
	Chunk(Chunk&& other) noexcept
		: chunk_bounds_(std::move(other.chunk_bounds_)) {
	}

	Chunk& operator=(const Chunk& other) {
		if (this != &other) {
			for (const auto& o : other.objects_) {
				objects_.push_back(o->clone());
			}
		}
		return *this;
	}

	Chunk& operator=(Chunk&& other) noexcept {
		return *this;
	}

	const BoundingBox& get_bounds()const;


	void render_chunk()const;
	void update_chunk();

	void add_object(std::unique_ptr<Object> object);
	void move_object(std::unique_ptr<Object> object);

private:
	AABBComparator& cmp_ = AABBComparator::get_instance();
	void sort_objects();
	std::vector<std::unique_ptr<Object>> objects_;
	const BoundingBox chunk_bounds_;
};