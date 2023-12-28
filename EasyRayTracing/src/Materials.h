#pragma once
#include "utility.h"
#include "Ray.h"
#include "Objects.h"
#include "Texture.h"

class Material
{
public:
	Material() = default;
	virtual bool scatter(const Ray& ray, HitRecord& rec, glm::vec3& alb, Ray& scatteredray, float& pdf) = 0;
	virtual float scatter_pdf(const Ray& ray, HitRecord& rec, Ray& scatteredray) const { return 1.0f; } // sampling pdf
	virtual glm::vec3& GetAlbedo() = 0;

	virtual std::shared_ptr<Texture> GetTex() const = 0;
	virtual void BindTex(std::shared_ptr<Texture> _tex) = 0;
	virtual glm::vec3 emitted(float u, float v, const glm::vec3& pos) const = 0;

private:
	std::shared_ptr<Texture> tex;
};

class Lambertian : public Material
{
public:
	Lambertian(glm::vec3 albedo = {1.0f, 1.0f, 1.0f});

	virtual bool scatter(const Ray& ray, HitRecord& rec, glm::vec3& alb, Ray& scatteredray, float& pdf) override;
	virtual glm::vec3& GetAlbedo() override { return albedo; }
	virtual float scatter_pdf(const Ray& ray, HitRecord& rec, Ray& scatteredray) const override; // sampling pdf

	virtual std::shared_ptr<Texture> GetTex() const override { return tex; }
	virtual void BindTex(std::shared_ptr<Texture> _tex) override { tex = _tex; }
	virtual glm::vec3 emitted(float u, float v, const glm::vec3& pos) const override { return glm::vec3{ 0.0f }; }

private:
	glm::vec3 albedo;
	float scatter_rate = 0.7f;
	std::shared_ptr<Texture> tex;
};

class Metal : public Material
{
public:
	Metal(glm::vec3 albedo = { 1.0f, 1.0f, 1.0f }, float fuzziness = 0.1f);

	virtual bool scatter(const Ray& ray, HitRecord& rec, glm::vec3& alb, Ray& scatteredray, float& pdf) override;

	virtual glm::vec3& GetAlbedo() override  { return albedo; }

	virtual std::shared_ptr<Texture> GetTex() const override { return tex; }
	virtual void BindTex(std::shared_ptr<Texture> _tex) override { tex = _tex; }

	virtual glm::vec3 emitted(float u, float v, const glm::vec3& pos) const override { return glm::vec3{ 0.0f }; }

private:
	glm::vec3 albedo;
	float fuzziness;
	std::shared_ptr<Texture> tex;
};

class Dielectric : public Material
{
public:
	Dielectric(glm::vec3 albedo = { 1.0f, 1.0f, 1.0f }, float refraction = 1.0f, float fuzziness = 0.05f);
	
	virtual bool scatter(const Ray& ray, HitRecord& rec, glm::vec3& alb, Ray& scatteredray, float& pdf) override;

	virtual glm::vec3& GetAlbedo() override { return albedo; }
	virtual std::shared_ptr<Texture> GetTex() const override { return tex; }
	virtual void BindTex(std::shared_ptr<Texture> _tex) override { tex = _tex; }

	virtual glm::vec3 emitted(float u, float v, const glm::vec3& pos) const override { return glm::vec3{ 0.0f }; }

	static float reflectance(float cosine, float ref_ratio);

private:
	glm::vec3 albedo;
	float refraction;
	float fuzziness;
	std::shared_ptr<Texture> tex;
};

class Emit : public Material
{
public:
	Emit(std::shared_ptr<Material> mat, glm::vec3 Emitalbedo = {1.0f, 1.0f, 1.0f});

	virtual bool scatter(const Ray& ray, HitRecord& rec, glm::vec3& alb, Ray& scatteredray, float& pdf) override;
	virtual glm::vec3& GetAlbedo() override { return Emitalbedo; }

	virtual std::shared_ptr<Texture> GetTex() const override { return tex; }
	virtual void BindTex(std::shared_ptr<Texture> _tex) override { tex = _tex; }

	virtual glm::vec3 emitted(float u, float v, const glm::vec3& pos) const override;
	std::shared_ptr<Material> GetInnerMat() const { return m_SurfaceMat; }
private:
	glm::vec3 Emitalbedo;
	std::shared_ptr<Material> m_SurfaceMat;
	std::shared_ptr<Texture> tex;

};

class Isotropic : public Material
{
	// unfinished
public:
	Isotropic(glm::vec3 albedo = { 1.0f, 1.0f, 1.0f });

	virtual bool scatter(const Ray& ray, HitRecord& rec, glm::vec3& alb, Ray& scatteredray, float& pdf) override;
	virtual glm::vec3& GetAlbedo() override { return albedo; }
	virtual float scatter_pdf(const Ray& ray, HitRecord& rec, Ray& scatteredray) const override; // sampling pdf

	virtual std::shared_ptr<Texture> GetTex() const override { return tex; }
	virtual void BindTex(std::shared_ptr<Texture> _tex) override { tex = _tex; }
	virtual glm::vec3 emitted(float u, float v, const glm::vec3& pos) const override { return glm::vec3{ 0.0f }; }

private:
	glm::vec3 albedo;
	std::shared_ptr<Texture> tex;
};