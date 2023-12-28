#include "Objects.h"

// ObjectTable-------------
void ObjectTable::Add(std::shared_ptr<Object> obj)
{
    m_Objects.push_back(obj);
    m_BoundingBox = AABB(m_BoundingBox, obj->GetBB());
}

void ObjectTable::Add(std::shared_ptr<ObjectTable> objtable)
{
    for (auto objptr : objtable->GetTable())
    {
        Add(objptr);
    }
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

// Sphere-------------------

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

// Quad -------------------

Quad::Quad(std::shared_ptr<Material> mat, const glm::vec3& origin, const glm::vec3& u, const glm::vec3& v)
    :m_Origin(origin), m_U(u), m_V(v), m_Material(mat)
{
    m_BoundingBox = AABB(origin, origin+u+v).Pad();

    glm::vec3 n = glm::cross(u, v);
    area = n.length();
    m_Normal = glm::normalize(n);  // we assume the normal is pointing the outside of the quad
    m_D = glm::dot(m_Normal, m_Origin);
    w = n / glm::dot(n, n);
}

Quad::Quad(const glm::vec3& origin, const glm::vec3& u, const glm::vec3& v, std::shared_ptr<Material> mat)
    :m_U(u), m_V(v), m_Origin(origin), m_Material(mat)
{
    m_BoundingBox = AABB(origin, origin + u + v).Pad();

    glm::vec3 n = glm::cross(u, v);
    area = n.length();
    m_Normal = glm::normalize(n);  // we assume the normal is pointing the outside of the quad
    m_D = glm::dot(m_Normal, m_Origin);
    w = n / glm::dot(n, n);
}

bool Quad::Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const
{
    // formular : dot(n, O) + t * dot(n, dir) = D
    auto denom = glm::dot(m_Normal, ray.GetDir());
    if (fabsf(denom) < 0.000001f)
    {
        // parallel
        return false;
    }

    float t = (m_D - glm::dot(m_Normal, ray.GetOrigin())) / denom;
    if (!ray_t.Contains(t)) return false; // not in ray range

    // check if the intersection is in the quad
    glm::vec3 intersection = ray.look_at(t);
    glm::vec3 p = intersection - m_Origin;
    float alpha = glm::dot(w, glm::cross(p, m_V)); // cord U
    float beta = glm::dot(w, glm::cross(m_U, p));  // cord V

    if ((alpha < 0) || (1 < alpha) || (beta < 0) || (1 < beta))  return false; // not in quad

    // update HitRecord
    if (t < rec.t)
    {
        rec.t = t;
        rec.u = alpha;
        rec.v = beta;
        rec.position = intersection;
        rec.normal = glm::dot(ray.GetDir(), m_Normal) < 0 ? m_Normal : -m_Normal;
        rec.mat_ptr = m_Material;
        // rec.front_face = glm::dot(ray.GetDir(), m_Normal) < 0 ? true : false;
        rec.front_face = true;
    }

    return true;
}

float Quad::pdf_value(const glm::vec3& origin, const glm::vec3& direction) const
{
    HitRecord rec;
    if (this->Hit(Ray(origin, direction), Interval(0.001f, Utility::FLOAT_MAX), rec)) // don't hit the object
        return 0;

    float distance_squared = rec.t * rec.t * direction.length() * direction.length();
    float cosine = fabsf(glm::dot(direction, rec.normal) / direction.length());

    return distance_squared / (cosine * area);
}

glm::vec3 Quad::random(const glm::vec3& origin) const
{
    auto p = m_Origin + (Utility::RandomFloat() * m_U) + (Utility::RandomFloat() * m_V);
    return p - origin;
}

Box::Box(const glm::vec3& a, const glm::vec3& b, std::shared_ptr<Material> mat)
{
    m_Objects = std::vector<std::shared_ptr<Object>>();

    auto min = glm::vec3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    auto max = glm::vec3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    auto dx = glm::vec3(max.x - min.x, 0, 0);
    auto dy = glm::vec3(0, max.y - min.y, 0);
    auto dz = glm::vec3(0, 0, max.z - min.z);

    m_Objects.push_back(std::make_shared<Quad>(glm::vec3(min.x, min.y, max.z), dx, dy, mat));  // front
    m_Objects.push_back(std::make_shared<Quad>(glm::vec3(max.x, min.y, max.z), -dz, dy, mat)); // right
    m_Objects.push_back(std::make_shared<Quad>(glm::vec3(max.x, min.y, min.z), -dx, dy, mat)); // back
    m_Objects.push_back(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dz, dy, mat));  // left
    m_Objects.push_back(std::make_shared<Quad>(glm::vec3(min.x, max.y, max.z), dx, -dz, mat)); // top
    m_Objects.push_back(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dx, dz, mat));  // bottom
}

void Box::translate(glm::vec3 displacement)
{
    for (int i = 0; i < m_Objects.size(); i++)
    {
        m_Objects[i] = std::make_shared<Translate>(m_Objects[i], displacement);
    }
}

void Box::rotateY(float degree)
{
    for (int i = 0; i < m_Objects.size(); i++)
    {
        m_Objects[i] = std::make_shared<RotateY>(m_Objects[i], degree);
    }
}

Translate::Translate(std::shared_ptr<Object> p, const glm::vec3& displacement)
    :m_Object(p), m_Offset(displacement)
{
    m_BoundingBox = p->GetBB() + displacement;
}

bool Translate::Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const
{
    Ray offset_ray(ray.GetOrigin() - m_Offset, ray.GetDir());  // backward move rays
    if (!m_Object->Hit(offset_ray, ray_t, rec)) { return false; }

    rec.position += m_Offset;
    return true;
}

RotateY::RotateY(std::shared_ptr<Object> p, float degree)
    :m_Object(p)
{
    float theta = Utility::DegreetoRadience(degree);
    sin_theta = sinf(theta);
    cos_theta = cosf(theta);

    m_BoundingBox = m_Object->GetBB();

    // modify the vertex to rotated space and measure the AABB

    glm::vec3 min(Utility::FLOAT_MAX, Utility::FLOAT_MAX, Utility::FLOAT_MAX);
    glm::vec3 max(-Utility::FLOAT_MAX, -Utility::FLOAT_MAX, -Utility::FLOAT_MAX);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                // traverse vertex
                auto x = i * m_BoundingBox.x.max + (1 - i) * m_BoundingBox.x.min;
                auto y = j * m_BoundingBox.y.max + (1 - j) * m_BoundingBox.y.min;
                auto z = k * m_BoundingBox.z.max + (1 - k) * m_BoundingBox.z.min;

                auto newx = cos_theta * x + sin_theta * z;
                auto newz = -sin_theta * x + cos_theta * z;

                glm::vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    m_BoundingBox = AABB(min, max);
}

bool RotateY::Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const
{
    auto origin = ray.GetOrigin();
    auto dir = ray.GetDir();

    // ray rotate in reversed direction
    origin[0] = cos_theta * ray.GetOrigin()[0] - sin_theta * ray.GetOrigin()[2];
    origin[2] = sin_theta * ray.GetOrigin()[0] + cos_theta * ray.GetOrigin()[2];

    dir[0] = cos_theta * ray.GetDir()[0] - sin_theta * ray.GetDir()[2];
    dir[2] = sin_theta * ray.GetDir()[0] + cos_theta * ray.GetDir()[2];

    Ray rotated_ray(origin, glm::normalize(dir));

    if (!m_Object->Hit(rotated_ray, ray_t, rec))
    {
        return false;
    }

    // rotate from object coord to world coord
    glm::vec3 position = rec.position;
    position[0] =  cos_theta * rec.position[0] + sin_theta * rec.position[2];
    position[2] = -sin_theta * rec.position[0] + cos_theta * rec.position[2];

    glm::vec3 normal = rec.normal;
    normal[0] =  cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
    normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

    rec.position = position;
    rec.normal = glm::normalize(normal);

    return true;
}
