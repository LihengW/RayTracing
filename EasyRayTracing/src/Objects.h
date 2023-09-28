#pragma once
#include "Ray.h"
#include "utility.h"
#include "AABB.h"
#include "Materials.h"
#include <vector>
#include <memory>

class Object
{
public:
	virtual bool Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const = 0;
	virtual AABB GetBB() const = 0;
};

class ObjectTable
{
public:
	inline size_t GetSize() const { return m_Objects.size(); }
	inline AABB GetBB() const { return m_BoundingBox; }
	
	void Add(std::shared_ptr<Object> obj);
	void Add(std::shared_ptr<ObjectTable> objtable);

	bool Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const;

	virtual const std::vector<std::shared_ptr<Object>>& GetTable() { return m_Objects; }

private:
	std::vector<std::shared_ptr<Object>>  m_Objects;
	AABB m_BoundingBox;
};

// specific Objects
class Sphere : public Object
{
public:
	Sphere(const glm::vec3& origin = {0.0f, 0.0f , 0.0f}, float radius = 0.0f);
	Sphere(std::shared_ptr<Material> material, const glm::vec3& origin = {0.0f, 0.0f , 0.0f}, float radius = 1.0f);

	inline const glm::vec3& GetOrigin() { return m_Origin; };
	inline const float& GetRadius() { return m_Radius; }

	virtual bool Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override;
	virtual AABB GetBB() const override { return m_BoundingBox; }

	void GetUVcord(const glm::vec3& pos, float& u, float& v) const;

	void SetUVOffset(float u, float v);

	void SetBB(AABB& box) { m_BoundingBox = box; } // dangerous function

private:
	glm::vec3 m_Origin;
	std::shared_ptr<Material> m_Material;
	float m_Radius;
	AABB m_BoundingBox;

	float u_offset;
	float v_offset;
};


class Quad : public Object
{
public:
	Quad(std::shared_ptr<Material> mat, const glm::vec3& origin = { 0.0f, 0.0f , 0.0f }, const glm::vec3& u = { 0.0f, 0.0f , 0.0f }, const glm::vec3& v = { 0.0f, 0.0f , 0.0f });
	Quad(const glm::vec3& origin, const glm::vec3& u, const glm::vec3& v, std::shared_ptr<Material> mat);
	virtual bool Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override;
	virtual AABB GetBB() const override { return m_BoundingBox; }

private:
	glm::vec3 m_Origin;
	glm::vec3 m_U;
	glm::vec3 m_V;

	glm::vec3 w;

	// quad plane attributes 
	glm::vec3 m_Normal;
	float m_D;

	std::shared_ptr<Material> m_Material;

	AABB m_BoundingBox;
};


class Box : public ObjectTable
{
public:
	Box(const glm::vec3& a, const glm::vec3& b, std::shared_ptr<Material> mat);
	virtual const std::vector<std::shared_ptr<Object>>& GetTable() override { return m_Objects; }

	void translate(glm::vec3 displacement);
	void rotateY(float degree);


private:
	std::vector<std::shared_ptr<Object>>  m_Objects;
};


// Transformation (Object Style)

class Translate : public Object
{
public:
	Translate(std::shared_ptr<Object> p, const glm::vec3& displacement);
	virtual bool Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override;
	virtual AABB GetBB() const override { return m_BoundingBox; }
	
private:
	std::shared_ptr<Object> m_Object;
	glm::vec3 m_Offset;
	AABB m_BoundingBox;
};


class RotateY : public Object
{
	// degree should be counter clock !
public:
	RotateY(std::shared_ptr<Object> p, float degree);
	virtual bool Hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override;
	virtual AABB GetBB() const override { return m_BoundingBox; }

private:
	std::shared_ptr<Object> m_Object;
	float sin_theta;
	float cos_theta;
	AABB m_BoundingBox;
};