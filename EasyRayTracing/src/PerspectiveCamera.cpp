#include "PerspectiveCamera.h"

#include <vendor/glm/glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float neardistance, float fardistance)
	:m_ProjectionMatrix(glm::perspective(fov, aspect, neardistance, fardistance)), m_ViewMatrix(1.0f)
{
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; // OpenGL order (column order)
}

void PerspectiveCamera::RecaiculateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
	// translate is for postion movement;
	m_ViewMatrix = glm::inverse(transform);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::SetProjection(float fov, float aspect, float neardistance, float fardistance)
{
	m_ProjectionMatrix = glm::perspective(fov, aspect, neardistance, fardistance);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
