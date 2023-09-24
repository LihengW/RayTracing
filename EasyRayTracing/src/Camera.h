#pragma once
#include "Ray.h"
#include "utility.h"


class Camera
{
public:
	Camera(glm::vec3 origin, glm::vec3 lookat, glm::vec3 up, float width, float aspectratio, float vfov, float focus_dist, float len_radius);

	Ray GetPerspectiveRay(float u, float v);
	// Ray GetOrthoRay(float u, float v);

	inline glm::vec3 GetOrigin() { return m_Origin; }

private:
	glm::vec3 m_Origin;
	float m_AspectRatio;
	float m_Vfov; // Vertical view angle (field of view)

	float m_FocusDist;
	float m_LenRadius;

	glm::vec3 m_Right;
	glm::vec3 m_LookDir;
	glm::vec3 m_Up;

	glm::vec3 viewport_u;
	glm::vec3 viewport_v;

	float m_Horizontal;
	float m_Vertical;
};