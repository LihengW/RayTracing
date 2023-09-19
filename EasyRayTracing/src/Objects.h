#pragma once
#include "Ray.h"
#include <vector>
#include <memory>

struct HitRecord
{
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 normal{ 0.0f, 0.0f, 0.0f };
	float t = std::numeric_limits<float>::max();
};


class Object
{
public:
	virtual bool hit(const Ray& ray, HitRecord& rec, float t_min, float t_max) const = 0;
};

class ObjectTable
{
public:
	inline size_t GetSize() const { return m_Objects.size(); }
	
	void Add(std::shared_ptr<Object> obj);

	bool hit(const Ray& ray, HitRecord& rec, float t_min = 0.0f, float t_max = std::numeric_limits<float>::max()) const;

private:
	std::vector<std::shared_ptr<Object>>  m_Objects;
};

class Sphere : public Object
{
public:
	Sphere(const glm::vec3& origin, float radius);

	inline const glm::vec3& GetOrigin() { return m_Origin; };
	inline const float& GetRadius() { return m_Radius; }

	virtual bool hit(const Ray& ray, HitRecord& rec, float t_min = 0.0f, float t_max = std::numeric_limits<float>::max()) const override;

private:
	glm::vec3 m_Origin;
	float m_Radius;
};