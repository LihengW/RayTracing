#include "Objects.h"


Sphere::Sphere(const glm::vec3& origin, float radius)
	:m_Origin(origin), m_Radius(radius)
{
    glm::vec3 rad_vec{ radius + 5.0f };
    //m_BoundingBox = AABB();
    m_BoundingBox = AABB(origin - rad_vec, origin + rad_vec);
}

Sphere::Sphere(std::shared_ptr<Material> material, const glm::vec3& origin, float radius)
    :m_Origin(origin), m_Radius(radius), m_Material(material)
{
    glm::vec3 rad_vec{ radius + 5.0f };
    // m_BoundingBox = AABB();
    m_BoundingBox = AABB(origin - rad_vec, origin + rad_vec);
}

bool Sphere::Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const
{
    bool Hit = false;
    glm::vec3 o_minus = ray.GetOrigin() - m_Origin;
    float c = glm::dot(o_minus, o_minus) - powf(m_Radius, 2);
    float b = 2 * glm::dot(o_minus, ray.GetDir());
    float delta = b * b - 4 * c;
    if (delta == 0)
    {
        float t = -1 * b / 2;
        if (t > ray_t.min && t > 0 && t < ray_t.max && t < rec.t)
        {
            rec.t = t;
            rec.position = ray.look_at(t);
            rec.normal = glm::normalize(rec.position - m_Origin);
            rec.color = m_Material->GetAlbedo();
            rec.mat_ptr = m_Material;
            if (glm::dot(rec.normal, ray.GetDir()) < 0) rec.front_face = true;
            Hit = true;
        }
    }
    else if (delta > 0)
    {
        float t;
        float t1 = (-1 * b + sqrt(delta)) / 2;
        float t2 = (-1 * b - sqrt(delta)) / 2;

        if (t1 > t2 && t1 >= 0) t = t2 >= 0 ? t2 : t1;
        else if (t2 > t1 && t2 >= 0) t = t1 >= 0 ? t1 : t2;
        else t = -1;

        if (t > ray_t.min && t > 0 && t < ray_t.max && t < rec.t)
        {
            rec.t = t;
            rec.position = ray.look_at(t);
            rec.normal = glm::normalize(rec.position - m_Origin);
            rec.color = m_Material->GetAlbedo();
            rec.mat_ptr = m_Material;
            if (glm::dot(rec.normal, ray.GetDir()) < 0) rec.front_face = true;
            Hit = true;
        }
    }

    return Hit;
}


void ObjectTable::Add(std::shared_ptr<Object> obj)
{
    m_Objects.push_back(obj);
    m_BoundingBox = AABB(m_BoundingBox, obj->GetBB());
}

bool ObjectTable::Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const
{
    bool Hit = false;
    for (auto obj : m_Objects)
    {
        Hit |= obj->Hit(ray, ray_t, rec);
    }
    return Hit;
}