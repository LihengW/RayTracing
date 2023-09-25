#pragma once
#include "utility.h"
#include "Ray.h"

class Interval {
public:
	Interval();
	Interval(float num1, float num2);
	Interval(const Interval& interval1, const Interval& interval2);

	float GetSize() const { return max - min; };
	Interval expand(float delta) { return Interval(min - delta / 2.0f, max + delta / 2.0f); }
public:
	float min, max;
};

class AABB {
public:
	Interval x, y, z;

	AABB() = default;
	AABB(const Interval& x, const Interval& y, const Interval& z);
	AABB(const glm::vec3& vector1, const glm::vec3& vector2);
	AABB(const AABB& box1, const AABB& box2);

	inline const Interval& GetAxis(int n) const {
		if (n == 0) return x;
		else if (n == 1) return y;
		else return z;
	}

	bool Hit(const Ray& ray, Interval ray_t) const;
};