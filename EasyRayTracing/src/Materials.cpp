#include "Materials.h"

Lambertian::Lambertian(glm::vec3 albedo)
	:albedo(albedo)
{}

bool Lambertian::scatter(const Ray& ray, HitRecord& rec, glm::vec3& attenuation, Ray& newray)
{
	if (Utility::RandomFloat() <= scatter_rate)
	{
		glm::vec3 newdir;
		do { newdir = Utility::RandomUnitVector(); } while (glm::dot(newdir, rec.normal) < 0);
		attenuation = albedo;
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
	attenuation = albedo;
	newray.SetOrigin(rec.position + 0.0001f * rec.normal);
	glm::vec3 newdir = ray.GetDir() - 2.0f * glm::dot(rec.normal, ray.GetDir()) * rec.normal;
	newdir = glm::normalize(fuzziness * Utility::RandomUnitVector() + newdir);
	newray.SetDir(newdir);
	return true;
}
