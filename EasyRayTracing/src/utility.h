#pragma once
#include <cmath>
#include <cstdlib>
#include <limits>
#include <functional>
#include <random>
#include "vendor/glm/glm/glm.hpp"

class Material;


class Utility
{
public:
	Utility();

	inline static float RandomFloat() { return rand() / (float)RAND_MAX; };
	inline static int RandomInt() { return rand(); };
	inline static float RandomFloat(float low, float high) { return low + (high - low) * RandomFloat(); }
	inline static glm::vec3 RandomVec3() { return glm::vec3{ RandomFloat(), RandomFloat(), RandomFloat() }; };
	static glm::vec3 RandomCosDir();

	inline static float clamp(float x, float min, float max) { if (x < min) return min; if (x > max) return max; return x; }
	static glm::vec3 clamp(glm::vec3 vec, float min, float max);
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

struct HitRecord
{
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 normal{ 0.0f, 0.0f, 0.0f };

	std::shared_ptr<Material> mat_ptr;

	float u, v;

	float t = Utility::FLOAT_MAX;
	bool front_face = false;

	inline void reset()
	{
		t = Utility::FLOAT_MAX;
		front_face = false;
	}
};

class ONB 
{
public:
	ONB() = default;
	ONB(const glm::vec3& w);

	glm::vec3 operator[](int i) const { return axis[i]; }
	glm::vec3& operator[](int i) { return axis[i]; }

	glm::vec3 u() const { return axis[0]; }
	glm::vec3 v() const { return axis[1]; }
	glm::vec3 w() const { return axis[2]; }

	glm::vec3 local(float a, float b, float c) const {
		return a * u() + b * v() + c * w();
	}

	glm::vec3 local(const glm::vec3& a) const {
		return a.x * u() + a.y * v() + a.z * w();
	}
	
	// w is the surface normal vector
	void build_from_w(const glm::vec3& w) {
		glm::vec3 unit_w = glm::normalize(w);
		glm::vec3 a = (fabs(unit_w.x) > 0.9) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0); 
		glm::vec3 v = glm::normalize(glm::cross(unit_w, a));
		glm::vec3 u = glm::cross(unit_w, v);
		axis[0] = u;
		axis[1] = v;
		axis[2] = unit_w;
	}

public:
	glm::vec3 axis[3];
};

