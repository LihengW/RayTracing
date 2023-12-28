#include "Materials.h"

Lambertian::Lambertian(glm::vec3 albedo)
	:albedo(albedo)
{}

bool Lambertian::scatter(const Ray& ray, HitRecord& rec, glm::vec3& alb, Ray& scatteredray, float& pdf)
{
	if (!rec.front_face)
	{
		return false;
	}
	if (Utility::RandomFloat() <= scatter_rate)
	{
		if (tex != nullptr)
		{
			alb = tex->GetValue(rec.u, rec.v, rec.position);
		}
		else
		{
			alb = albedo;
		}

		ONB uvw;
		uvw.build_from_w(rec.normal);
		glm::vec3 scatter_direction = uvw.local(Utility::RandomCosDir());
		glm::vec3 newdir;
		do { newdir = Utility::RandomUnitVector(); } while (glm::dot(newdir, rec.normal) < 0);
		scatteredray.SetOrigin(rec.position + 0.0001f * rec.normal);
		scatteredray.SetDir(glm::normalize(newdir + rec.normal));
		float pdf = glm::dot(uvw.w(), scatteredray.GetDir()) / Utility::pi;
		return true;
	}
	else
	{
		return false;
	}
	
}

float Lambertian::scatter_pdf(const Ray& ray, HitRecord& rec, Ray& scatteredray) const
{
	auto cos_theta = glm::dot(rec.normal, glm::normalize(scatteredray.GetDir()));
	return cos_theta < 0 ? 0 : cos_theta / Utility::pi;
}

Metal::Metal(glm::vec3 albedo, float fuzziness)
	:albedo(albedo), fuzziness(fuzziness)
{}

bool Metal::scatter(const Ray& ray, HitRecord& rec, glm::vec3& alb, Ray& scatteredray, float& pdf)
{
	if (!rec.front_face)
	{
		return false;
	}

	if (tex != nullptr)
	{
		alb = tex->GetValue(rec.u, rec.v, rec.position);
	}
	else
	{
		alb = albedo;
	}

	scatteredray.SetOrigin(rec.position + 0.0001f * rec.normal);
	glm::vec3 newdir = ray.GetDir() - 2.0f * glm::dot(rec.normal, ray.GetDir()) * rec.normal;
	newdir = glm::normalize(fuzziness * Utility::RandomUnitVector() + newdir);
	scatteredray.SetDir(newdir);
	return true;
}

Dielectric::Dielectric(glm::vec3 albedo, float refraction, float fuzziness)
	:refraction(refraction), albedo(albedo), fuzziness(fuzziness)
{
	;
}

bool Dielectric::scatter(const Ray& ray, HitRecord& rec, glm::vec3& alb, Ray& scatteredray, float& pdf)
{
	if (tex != nullptr)
	{
		alb = tex->GetValue(rec.u, rec.v, rec.position);
	}
	else
	{
		alb = albedo;
	}
	float refraction_ratio;
	float cos_theta;
	if (rec.front_face)
	{
		refraction_ratio = 1.0f / refraction;
		cos_theta = glm::dot(-ray.GetDir(), rec.normal);
	}
	else
	{
		refraction_ratio = refraction;
		cos_theta = glm::dot(ray.GetDir(), rec.normal);
	}

	float sin_theta = sqrt(1 - cos_theta * cos_theta);

	if (sin_theta * refraction_ratio > 1.0f || reflectance(cos_theta, refraction_ratio) > Utility::RandomFloat())
	{
		// reflect
		// glm::vec3 newdir = ray.GetDir() - 2.0f * glm::dot(rec.normal, ray.GetDir()) * rec.normal;
		if (rec.front_face)
		{
			glm::vec3 newdir = glm::reflect(ray.GetDir(), rec.normal);
			newdir = glm::normalize(fuzziness * Utility::RandomUnitVector() + newdir);
			scatteredray.SetDir(newdir);
			scatteredray.SetOrigin(rec.position + 0.0001f * rec.normal);
		}
		else
		{
			glm::vec3 newdir = glm::reflect(ray.GetDir(), -rec.normal);
			newdir = glm::normalize(fuzziness * Utility::RandomUnitVector() + newdir);
			scatteredray.SetDir(newdir);
			scatteredray.SetOrigin(rec.position - 0.0001f * rec.normal);
		}
	}
	else
	{
		glm::vec3 newdir;
		if (rec.front_face)
		{
			scatteredray.SetOrigin(rec.position - 0.0001f * rec.normal);
			newdir = glm::normalize(glm::refract(ray.GetDir(), rec.normal, refraction_ratio) + fuzziness * Utility::RandomUnitVector());
		}
		else
		{
			scatteredray.SetOrigin(rec.position + 0.0001f * rec.normal);
			newdir = glm::normalize(glm::refract(ray.GetDir(), -rec.normal, refraction_ratio) + fuzziness * Utility::RandomUnitVector());
		}
		scatteredray.SetDir(newdir);
	}

	return true;
}

float Dielectric::reflectance(float cosine, float ref_ratio)
{
	auto r0 = (1.0f - ref_ratio) / (1.0f + ref_ratio);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * powf((1.0f - cosine), 5);
}

Emit::Emit(std::shared_ptr<Material> mat, glm::vec3 Emitalbedo)
	:m_SurfaceMat(mat), Emitalbedo(Emitalbedo)
{
	;
}

glm::vec3 Emit::emitted(float u, float v, const glm::vec3& pos) const
{
	return Emitalbedo;
}

bool Emit::scatter(const Ray& ray, HitRecord& rec, glm::vec3& alb, Ray& scatteredray, float& pdf)
{
	return m_SurfaceMat->scatter(ray, rec, alb, scatteredray, pdf);
}

Isotropic::Isotropic(glm::vec3 albedo)
{
	this->albedo = albedo;
}

bool Isotropic::scatter(const Ray& ray, HitRecord& rec, glm::vec3& alb, Ray& scatteredray, float& pdf)
{
	return false;
}

float Isotropic::scatter_pdf(const Ray& ray, HitRecord& rec, Ray& scatteredray) const
{
	return 0.0f;
}
