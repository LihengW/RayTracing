#include "utility.h"

const float Utility::infinity = std::numeric_limits<float>::infinity();
const float Utility::pi = 3.14159265358979f;

const float Utility::FLOAT_MAX = std::numeric_limits<float>::max();
const float Utility::FLOAT_MIN = std::numeric_limits<float>::min();

Utility::Utility()
{
	;
}

glm::vec3 Utility::RandomUnitVector()
{
	float theta = RandomFloat(0.0f, 2 * pi);
	float z = RandomFloat(-1.0f, 1.0f);
	float r = sqrt(1 - z * z);
	return glm::vec3(r*sin(theta), r*cos(theta), z);
}

glm::vec2 Utility::RandomDisk()
{
	float theta = RandomFloat(0.0f, 2 * pi);
	return glm::vec2(sin(theta), cos(theta));
}
