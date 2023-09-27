#include "Materials.h"

Lambertian::Lambertian(glm::vec3 albedo)
	:albedo(albedo)
{}

bool Lambertian::scatter(const Ray& ray, HitRecord& rec, glm::vec3& attenuation, Ray& newray)
{
	if (!rec.front_face)
	{
		return false;
	}
	if (Utility::RandomFloat() <= scatter_rate)
	{
		if (tex != nullptr)
		{
			attenuation = tex->GetValue(rec.u, rec.v, rec.position);
		}
		else
		{
			attenuation = albedo;
		}
		glm::vec3 newdir;
		do { newdir = Utility::RandomUnitVector(); } while (glm::dot(newdir, rec.normal) < 0);
		newray.SetOrigin(rec.position + 0.0001f * rec.normal);
		newray.SetDir(glm::normalize(newdir + rec.normal));
		return true;
	}
	else
	{
		return false;
	}
	
}

Metal::Metal(glm::vec3 albedo, float fuzziness)
	:albedo(albedo), fuzziness(fuzziness)
{}

bool Metal::scatter(const Ray& ray, HitRecord& rec, glm::vec3& attenuation, Ray& newray)
{
	if (!rec.front_face)
	{
		return false;
	}

	if (tex != nullptr)
	{
		attenuation = tex->GetValue(rec.u, rec.v, rec.position);
	}
	else
	{
		attenuation = albedo;
	}

	newray.SetOrigin(rec.position + 0.0001f * rec.normal);
	glm::vec3 newdir = ray.GetDir() - 2.0f * glm::dot(rec.normal, ray.GetDir()) * rec.normal;
	newdir = glm::normalize(fuzziness * Utility::RandomUnitVector() + newdir);
	newray.SetDir(newdir);
	return true;
}

Dielectric::Dielectric(glm::vec3 albedo, float refraction, float fuzziness)
	:refraction(refraction), albedo(albedo), fuzziness(fuzziness)
{
	;
}

bool Dielectric::scatter(const Ray& ray, HitRecord& rec, glm::vec3& attenuation, Ray& newray)
{
	if (tex != nullptr)
	{
		attenuation = tex->GetValue(rec.u, rec.v, rec.position);
	}
	else
	{
		attenuation = albedo;
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
			newray.SetDir(newdir);
			newray.SetOrigin(rec.position + 0.0001f * rec.normal);
		}
		else
		{
			glm::vec3 newdir = glm::reflect(ray.GetDir(), -rec.normal);
			newdir = glm::normalize(fuzziness * Utility::RandomUnitVector() + newdir);
			newray.SetDir(newdir);
			newray.SetOrigin(rec.position - 0.0001f * rec.normal);
		}
	}
	else
	{
		glm::vec3 newdir;
		if (rec.front_face)
		{
			newray.SetOrigin(rec.position - 0.0001f * rec.normal);
			newdir = glm::normalize(glm::refract(ray.GetDir(), rec.normal, refraction_ratio) + fuzziness * Utility::RandomUnitVector());
		}
		else
		{
			newray.SetOrigin(rec.position + 0.0001f * rec.normal);
			newdir = glm::normalize(glm::refract(ray.GetDir(), -rec.normal, refraction_ratio) + fuzziness * Utility::RandomUnitVector());
		}
		newray.SetDir(newdir);
	}

	return true;
}

float Dielectric::reflectance(float cosine, float ref_ratio)
{
	auto r0 = (1.0f - ref_ratio) / (1.0f + ref_ratio);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * powf((1.0f - cosine), 5);
}
