#pragma once
#include <iostream>
#include "Picture.h"
#include "Ray.h"
#include "Objects.h"
#include "utility.h"
#include "Camera.h"
#include "BVHnode.h"
#include "PDF.h"

class Renderer
{
public:
	Renderer(int width, int height, int channel, Camera& camera, std::shared_ptr<BVHnode> world, ObjectTable lights);
	Renderer(int width, int height, int channel, Camera& camera, ObjectTable world, ObjectTable lights);
	Renderer(int width, int height, int channel, Camera& camera, std::shared_ptr<BVHnode> world);

	void SetSampleperPixel(int n) { sample_per_pixel = n; }
	int GetSampleperPixel() { return sample_per_pixel; }

	void SetShadingDepth(int n) { shading_depth = n; }
	int GetShadingDepth() { return shading_depth; }

	void Render();

	glm::vec3 shading(Ray ray, HitRecord& rec, int depth);

private:
	int m_Width;
	int m_Height;
	int m_Channel;

	int shading_depth = 20;

	std::unique_ptr<Picture> m_Picture;
	int sample_per_pixel = 50;

	Camera m_Camera;
	
	std::shared_ptr<BVHnode> m_World;
	ObjectTable m_Lights;


	
};