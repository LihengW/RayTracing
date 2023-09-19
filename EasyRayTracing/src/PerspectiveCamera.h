#pragma once
#pragma once
#include <vendor/glm/glm/glm.hpp>

class PerspectiveCamera
{
public:
	PerspectiveCamera(float fov, float aspect, float neardistance, float fardistance); // parameters are the boundary of the screen;

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(const glm::vec3& position) { m_Position = position; RecaiculateViewMatrix(); }

	void SetProjection(float fov, float aspect, float neardistance, float fardistance);

	float GetRotation() const { return m_Rotation; }
	void SetRotation(float rotation) { m_Rotation = rotation; RecaiculateViewMatrix(); }

	const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
	const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

private:
	void RecaiculateViewMatrix();

private:
	// do projection
	glm::mat4 m_ProjectionMatrix;
	// oppisite of the camera movement
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	// in degrees
	float m_Rotation = 0.0f;
};