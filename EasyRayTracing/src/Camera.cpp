#include "Camera.h"

Camera::Camera(glm::vec3 origin, float horizontal, float vertical)
	:m_Origin(origin), m_Horizontal({ horizontal , 0.0f, 0.0f }), m_Vertical({ 0.0f, vertical, 0.0f })
{
	;
}

Ray Camera::GetPerspectiveRay(float u, float v)
{
	u = Utility::clamp(u, -1.0f, 1.0f);
	v = Utility::clamp(v, -1.0f, 1.0f);

	return Ray(m_Origin, glm::normalize(glm::vec3{0.0f, 0.0f, -1.0f} + u * m_Horizontal + v * m_Vertical));
}

Ray Camera::GetOrthoRay(float u, float v)
{
	u = Utility::clamp(u, -1.0f, 1.0f);
	v = Utility::clamp(v, -1.0f, 1.0f);
	return Ray(m_Origin + u * m_Horizontal + v * m_Vertical, glm::vec3{ 0.0f, 0.0f, -1.0f });
}



