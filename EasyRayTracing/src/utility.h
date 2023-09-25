#pragma once
#include <cmath>
#include <cstdlib>
#include <limits>
#include <functional>
#include <random>
#include "vendor/glm/glm/glm.hpp"

class Material;

struct HitRecord
{
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 normal{ 0.0f, 0.0f, 0.0f };
	glm::vec3 color{ 0.0f, 0.0f, 0.0f };
	std::shared_ptr<Material> mat_ptr;
	float t = std::numeric_limits<float>::max();
	bool front_face = false;

	inline void reset()
	{
		t = std::numeric_limits<float>::max();
		front_face = false;
	}
};

class Utility
{
public:
	Utility();

	inline static float RandomFloat() { return rand() / (float)RAND_MAX; };
	inline static int RandomInt() { return rand(); };
	inline static float RandomFloat(float low, float high) { return low + (high - low) * RandomFloat(); }
	inline static glm::vec3 RandomVec3() { return glm::vec3{ RandomFloat(), RandomFloat(), RandomFloat() }; };

	inline static float clamp(float x, float min, float max) { if (x < min) return min; if (x > max) return max; return x; }
	inline static float LineartoGamma(float linear_comp) { return sqrt(linear_comp); }

	inline static float DegreetoRadience(float degree) { return degree / 180.0f * pi; }
	inline static float RadiencetoDegree(float rad) { return rad / pi * 180.0f; }

	inline static float Min(float num1, float num2) { return num1 < num2 ? num1 : num2; }
	inline static float Max(float num1, float num2) { return num1 > num2 ? num1 : num2; }

	static  glm::vec3 RandomUnitVector();
	static glm::vec2 RandomDisk();
	
	static const float infinity;
	static const float pi;

	static const float FLOAT_MAX;
	static const float FLOAT_MIN;

};