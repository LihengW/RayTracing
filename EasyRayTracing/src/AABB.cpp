#include "AABB.h"

Interval::Interval()
	:min(-1000.0f), max(1000.0f)
{}

Interval::Interval(float num1, float num2)
{
	min = Utility::Min(num1, num2);
	max = Utility::Max(num1, num2);
}

Interval::Interval(const Interval& interval1, const Interval& interval2)
{
	min = Utility::Min(interval1.min, interval2.min);
	max = Utility::Max(interval1.max, interval2.max);
}

AABB::AABB(const Interval& x, const Interval& y, const Interval& z)
	:x(x), y(y), z(z)
{}

AABB::AABB(const glm::vec3 & vector1, const glm::vec3 & vector2)
{
	x = Interval(vector1.x, vector2.x);
	y = Interval(vector1.y, vector2.y);
	z = Interval(vector1.z, vector2.z);
}

AABB::AABB(const AABB& box1, const AABB& box2)
{
	x = Interval(box1.x, box2.x);
	y = Interval(box1.y, box2.y);
	z = Interval(box1.z, box2.z);
}

bool AABB::Hit(const Ray & ray, Interval ray_t) const
{
	for (int dim = 0; dim < 3; dim++)
	{
		float invD = 1.0f / ray.GetDir()[dim];
		auto orig = ray.GetOrigin()[dim];

		auto t0 = (GetAxis(dim).min - orig) * invD;
		auto t1 = (GetAxis(dim).max - orig) * invD;

		if (invD < 0) std::swap(t0, t1);

		if (t0 > ray_t.min) ray_t.min = t0;
		if (t1 < ray_t.max) ray_t.max = t1;

		if (ray_t.min >= ray_t.max) return false;
	}
	return true;
}


