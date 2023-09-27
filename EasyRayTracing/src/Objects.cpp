#include "Objects.h"


Sphere::Sphere(const glm::vec3& origin, float radius)
	:m_Origin(origin), m_Radius(radius), u_offset(0.0f), v_offset(0.0f)
{
    glm::vec3 rad_vec{ radius + 5.0f };
    //m_BoundingBox = AABB();
    m_BoundingBox = AABB(origin - rad_vec, origin + rad_vec);
}

Sphere::Sphere(std::shared_ptr<Material> material, const glm::vec3& origin, float radius)
    :m_Origin(origin), m_Radius(radius), m_Material(material), u_offset(0.0f), v_offset(0.0f)
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
            rec.mat_ptr = m_Material;
            GetUVcord(rec.position, rec.u, rec.v);
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
            rec.mat_ptr = m_Material;
            GetUVcord(rec.position, rec.u, rec.v);
            if (glm::dot(rec.normal, ray.GetDir()) < 0) rec.front_face = true;
            Hit = true;
        }
    }

    return Hit;
}

void Sphere::GetUVcord(const glm::vec3& pos, float& u, float& v) const
{
    // the pos must on the sphere surface
    glm::vec3 pos_origin = pos - m_Origin;
    if (glm::dot(pos_origin, pos_origin) - m_Radius * m_Radius <= 1.0f)
    {
        pos_origin = pos_origin / m_Radius;
        auto theta = acos(-pos_origin.y);
        auto phi = atan2(-pos_origin.z, pos_origin.x) + Utility::pi;
        u = phi / (2 * Utility::pi);
        v = theta / Utility::pi;

        u += u_offset;
        while (u > 1.0f) { u -= 1.0f; }
        while (u < 0.0f) { u += 1.0f; }

        v += v_offset;
        while (v > 1.0f) { v -= 1.0f; }
        while (v < 0.0f) { v += 1.0f; }
    }
    else
    {
        u = -1.0f;
        v = -1.0f;
    }
}

void Sphere::SetUVOffset(float u, float v)
{
    u_offset = u;
    v_offset = v;
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