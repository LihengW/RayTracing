#include "BVHnode.h"

BVHnode::BVHnode(const std::vector<std::shared_ptr<Object>>& src_objs, size_t start, size_t end)
{
	auto objects = src_objs; // copy
	size_t objects_num = end - start;

	int axis = Utility::RandomInt() % 3;
	auto comparator = (axis == 0) ? box_x_compare
		            : (axis == 1) ? box_y_compare
		            : box_z_compare;

	if (objects_num == 1) 
	{
		left = right = objects[start];
	}
	else if (objects_num == 2) 
	{
		if (comparator(objects[start], objects[start + 1])) {
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else 
	{
		std::sort(objects.begin() + start, objects.begin() + end, comparator);

		auto mid = start + objects_num / 2;
		left = std::make_shared<BVHnode>(objects, start, mid);
		right = std::make_shared<BVHnode>(objects, mid, end);
	}

	m_BoundingBox = AABB(left->GetBB(), right->GetBB());
}

bool BVHnode::Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const
{
	if (!m_BoundingBox.Hit(ray, ray_t)) return false;

	bool hit_left = left->Hit(ray, ray_t, rec);
	ray_t.max = hit_left ? rec.t : ray_t.max;
	bool hit_right = right->Hit(ray, ray_t, rec);

	return hit_left || hit_right;
}

AABB BVHnode::GetBB() const
{
	return m_BoundingBox;
}

