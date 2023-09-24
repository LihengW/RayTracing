#include "Camera.h"

Camera::Camera(glm::vec3 origin, glm::vec3 lookat, glm::vec3 up, float width, float aspectratio, float vfov, float focus_dist, float len_radius)
	:m_Origin(origin), m_Horizontal(width), m_AspectRatio(aspectratio), m_Vfov(vfov), m_FocusDist(focus_dist), m_LenRadius(len_radius)
{
	m_Vertical = width / aspectratio;
	m_LookDir = glm::normalize(lookat - origin);
	// up is relative
	m_Right = glm::normalize(glm::cross(m_LookDir, up));
	m_Up = glm::cross(m_Right, m_LookDir);
	
	// viewports calculation
	float theta = Utility::DegreetoRadience(vfov);
	float h = m_FocusDist * tanf(theta / 2);

	float viewport_height = 2 * h;
	float viewport_width = viewport_height * m_AspectRatio;

	viewport_v = m_Up * viewport_height / 2.0f;
	viewport_u = m_Right * viewport_width / 2.0f;
}

Ray Camera::GetPerspectiveRay(float u, float v)
{
	u = Utility::clamp(u, -1.0f, 1.0f);
	v = Utility::clamp(v, -1.0f, 1.0f);

	glm::vec3 orgin = m_Origin;
	glm::vec3 dir = u * viewport_u + v * viewport_v + m_LookDir * m_FocusDist;

	glm::vec2 rd = m_LenRadius * Utility::RandomDisk();
	glm::vec3 offset = rd.x * viewport_u + rd.y * viewport_v;

	return Ray(orgin + offset, glm::normalize(dir - offset));
}

//Ray Camera::GetOrthoRay(float u, float v)
//{
//	u = Utility::clamp(u, -1.0f, 1.0f);
//	v = Utility::clamp(v, -1.0f, 1.0f);
//	return Ray(m_Origin + u * m_Horizontal + v * m_Vertical, glm::vec3{ 0.0f, 0.0f, -1.0f });
//}



