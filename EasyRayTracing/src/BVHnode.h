#pragma once
#include "Objects.h"

class BVHnode : public Object
{
public:
    BVHnode(ObjectTable& objtable) : BVHnode(objtable.GetTable(), 0, objtable.GetTable().size()) {}

	BVHnode(const std::vector<std::shared_ptr<Object>>& src_objs, size_t start, size_t end);

	bool Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override;
	virtual AABB GetBB() const override;

private:
    static bool box_compare(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b, int axis_index) // compare on the left side (smaller one)
    {
        return a->GetBB().GetAxis(axis_index).min < b->GetBB().GetAxis(axis_index).min;
    }

    static bool box_x_compare(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b) {
        return box_compare(a, b, 2);
    }

	std::shared_ptr<Object> left;
	std::shared_ptr<Object> right;

	AABB m_BoundingBox;
};