#include "Perlin.h"

Perlin::Perlin()
{
	ranfloat = new float[point_count];
	for (int i = 0; i < point_count; i++)
	{
		ranfloat[i] = Utility::RandomFloat();
	}

	perm_x = perlin_generate_perm();
	perm_y = perlin_generate_perm();
	perm_z = perlin_generate_perm();
}

Perlin::~Perlin()
{
	delete[] ranfloat;
	delete[] perm_x;
	delete[] perm_y;
	delete[] perm_z;
}

float Perlin::noise(const glm::vec3& pos) const
{
	auto i = static_cast<int>(4 * pos.x) & 255; // 255 = 11111111
	auto j = static_cast<int>(4 * pos.y) & 255;
	auto k = static_cast<int>(4 * pos.z) & 255;

	return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
}

void Perlin::permute(int* p, int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		//exchange the p[i] with random int below i
		int target = Utility::RandomInt() % i;
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
}

int* Perlin::perlin_generate_perm()
{
	// It's like a interference
	auto p = new int[point_count];

	for (int i = 0; i < Perlin::point_count; i++)
		p[i] = i;

	permute(p, point_count);

	return p;
}

