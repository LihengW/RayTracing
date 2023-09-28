#pragma once
#include "utility.h"
#include "Ray.h"

class Interval {
public:
	Interval();
	Interval(float num1, float num2);
	Interval(const Interval& interval1, const Interval& interval2);

	float GetSize() const { return max - min; };
	inline bool Contains(float num) const { return num <= max && num >= min; }
	inline Interval Expand(float delta) const { return Interval(min - delta / 2.0f, max + delta / 2.0f); }
public:
	float min, max;

	friend Interval operator+(const Interval& ival, float displacement) {
		return Interval(ival.min + displacement, ival.max + displacement);
	}

	friend Interval operator+(float displacement, const Interval& ival) {
		return ival + displacement;
	}
};

class AABB {
public:
	Interval x, y, z;

	AABB() = default;
	AABB(const Interval& x, const Interval& y, const Interval& z);
	AABB(const glm::vec3& vector1, const glm::vec3& vector2);
	AABB(const AABB& box1, const AABB& box2);

	AABB Pad() const;

	inline const Interval& GetAxis(int n) const {
		if (n == 0) return x;
		else if (n == 1) return y;
		else return z;
	}

	bool Hit(const Ray& ray, Interval ray_t) const;

	friend AABB operator+(const AABB& bbox, const glm::vec3& offset)
	{
		return AABB(bbox.x + offset.x, bbox.y + offset.y, bbox.z + offset.z);
	}

	friend AABB operator+(const glm::vec3& offset, const AABB& bbox) {
		return bbox + offset;
	}
};