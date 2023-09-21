#pragma once
#include "vendor/glm/glm/glm.hpp"
#include "utility.h"

class Ray
{
public:
	Ray() = default;
	Ray(const glm::vec3& origin, const glm::vec3& dir)
		:m_Origin(origin)
	{
		m_Dir = glm::normalize(dir);
	}

	inline glm::vec3 look_at(float dist) const
	{
		return m_Origin + dist * m_Dir;
	}

	inline const glm::vec3& GetOrigin() const { return m_Origin; }
	inline const glm::vec3& GetDir() const { return m_Dir; }
	inline void SetOrigin(glm::vec3 origin) { m_Origin = origin; }
	inline void SetDir(glm::vec3 dir) { m_Dir = dir; }

private:
	glm::vec3 m_Origin;
	glm::vec3 m_Dir;

};

