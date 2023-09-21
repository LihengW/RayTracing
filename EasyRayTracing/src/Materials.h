#pragma once
#include "utility.h"
#include "Ray.h"
#include "Objects.h"

class Material
{
public:
	Material() = default;
	virtual bool scatter(const Ray& ray, HitRecord& rec, glm::vec3& attenuation, Ray& newray) = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(glm::vec3 albedo = {1.0f, 1.0f, 1.0f});

	virtual bool scatter(const Ray& ray, HitRecord& rec, glm::vec3& attenuation, Ray& newray) override;
	glm::vec3& GetAlbedo() { return albedo; }
private:
	glm::vec3 albedo;
	float scatter_rate = 0.7f;
};

class Metal : public Material
{
public:
	Metal(glm::vec3 albedo = { 1.0f, 1.0f, 1.0f }, float fuzziness = 0.1f);

	virtual bool scatter(const Ray& ray, HitRecord& rec, glm::vec3& attenuation, Ray& newray) override;
	glm::vec3& GetAlbedo() { return albedo; }
private:
	glm::vec3 albedo;
	float fuzziness;
};
