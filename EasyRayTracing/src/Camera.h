#pragma once
#include "Ray.h"
#include "utility.h"


class Camera
{
public:
	Camera(glm::vec3 origin, float horizontal, float vertical);

	Ray GetPerspectiveRay(float u, float v);
	Ray GetOrthoRay(float u, float v);

	inline glm::vec3 GetOrigin() { return m_Origin; }
	inline glm::vec2 GetSize() { return glm::vec2{ 2 * m_Horizontal.x, 2 * m_Vertical.y }; }

private:
	glm::vec3 m_Origin;
	glm::vec3 m_Horizontal;
	glm::vec3 m_Vertical;

	float m_AspectRatio;
	float vfov; // Vertical view angle (field of view)
};