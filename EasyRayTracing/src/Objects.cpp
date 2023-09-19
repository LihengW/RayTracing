#include "Objects.h"


Sphere::Sphere(const glm::vec3& origin, float radius)
	:m_Origin(origin), m_Radius(radius)
{
	;
}

bool Sphere::hit(const Ray& ray, HitRecord& rec, float t_min, float t_max) const
{
    bool hit = false;
    glm::vec3 o_minus = ray.GetOrigin() - m_Origin;
    float c = glm::dot(o_minus, o_minus) - pow(m_Radius, 2);
    float b = 2 * glm::dot(o_minus, ray.GetDir());
    float delta = b * b - 4 * c;
    if (delta == 0)
    {
        float t = -1 * b / 2;
        if (t > t_min && t > 0 && t < t_max && t < rec.t)
        {
            rec.t = t;
            rec.position = ray.look_at(t);
            rec.normal = glm::normalize(rec.position - m_Origin);
            hit = true;
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

        if (t > t_min && t > 0 && t < t_max && t < rec.t)
        {
            rec.t = t;
            rec.position = ray.look_at(t);
            rec.normal = glm::normalize(rec.position - m_Origin);
            hit = true;
        }
    }

    return hit;
}


void ObjectTable::Add(std::shared_ptr<Object> obj)
{
    m_Objects.push_back(obj);
}

bool ObjectTable::hit(const Ray& ray, HitRecord& rec, float t_min, float t_max) const
{
    bool hit = false;
    for (auto obj : m_Objects)
    {
        hit |= obj->hit(ray, rec, t_min, t_max);
    }
    return hit;
}