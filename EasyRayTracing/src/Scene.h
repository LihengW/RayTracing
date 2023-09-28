#pragma once
#include "utility.h"
#include "Camera.h"

class Scene
{
public:
	Scene() : m_Camera(nullptr) {}
	Scene(std::shared_ptr<Camera> camera) : m_Camera(camera) {}
	inline std::shared_ptr<Camera> GetCamera() { return m_Camera; }
	
private:
	std::shared_ptr<Camera> m_Camera;
};