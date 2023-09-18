#pragma once
#include "vendor/glm/glm/glm.hpp"

class Ray
{
public:
	Ray(const glm::vec3& origin, const glm::vec3& dir)
		:origin(origin), dir(dir)
	{
		;
	}

	glm::vec3 look_at(float dist)
	{
		return origin + dist * dir;
	}

private:
	glm::vec3 origin;
	glm::vec3 dir;

};