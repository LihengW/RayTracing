#pragma once
#include "Ray.h"
#include "utility.h"
#include "Materials.h"
#include <vector>
#include <memory>

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
	Sphere(const glm::vec3& origin = {0.0f, 0.0f , 0.0f}, float radius = 0.0f);
	Sphere(std::shared_ptr<Material> material, const glm::vec3& origin = {0.0f, 0.0f , 0.0f}, float radius = 1.0f);

	inline const glm::vec3& GetOrigin() { return m_Origin; };
	inline const float& GetRadius() { return m_Radius; }

	virtual bool hit(const Ray& ray, HitRecord& rec, float t_min = 0.0f, float t_max = std::numeric_limits<float>::max()) const override;

private:
	glm::vec3 m_Origin;
	std::shared_ptr<Material> m_Material;
	float m_Radius;
};