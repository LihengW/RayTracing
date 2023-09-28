#include "Perlin.h"

Perlin::Perlin()
{
	ranvec3 = new glm::vec3[point_count];
	for (int i = 0; i < point_count; i++)
	{
		ranvec3[i] = Utility::RandomUnitVector();
	}

	perm_x = perlin_generate_perm();
	perm_y = perlin_generate_perm();
	perm_z = perlin_generate_perm();
}

Perlin::~Perlin()
{
	delete[] ranvec3;
	delete[] perm_x;
	delete[] perm_y;
	delete[] perm_z;
}

float Perlin::noise(const glm::vec3& pos) const
{
	auto u = pos.x - floor(pos.x);
	auto v = pos.y - floor(pos.y);
	auto w = pos.z - floor(pos.z);

	auto i = static_cast<int>(floor(pos.x)); // 255 = 11111111
	auto j = static_cast<int>(floor(pos.y));
	auto k = static_cast<int>(floor(pos.z));

	glm::vec3 c[2][2][2];

	for (int di = 0; di < 2; di++)
		for (int dj = 0; dj < 2; dj++)
			for (int dk = 0; dk < 2; dk++)
				c[di][dj][dk] = ranvec3[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]]; // get value need to be interpolate

	return perlin_interp(c, u, v, w);
}

float Perlin::turb(const glm::vec3& pos, int depth) const
{
	auto accum = 0.0;
	auto temp_p = pos;
	auto weight = 1.0;

	for (int i = 0; i < depth; i++) {
		accum += weight * noise(temp_p);
		weight *= 0.5;
		temp_p *= 2;
	}

	return fabs(accum);
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

float Perlin::perlin_interp(glm::vec3 c[2][2][2], float u, float v, float w)
{
	auto uu = u * u * (3 - 2 * u);
	auto vv = v * v * (3 - 2 * v);
	auto ww = w * w * (3 - 2 * w);
	auto accum = 0.0;

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++) {
				glm::vec3 weight_v(u - i, v - j, w - k);
				accum += (i * uu + (1 - i) * (1 - uu))
					* (j * vv + (1 - j) * (1 - vv))
					* (k * ww + (1 - k) * (1 - ww))
					* dot(c[i][j][k], weight_v);
			}

	return accum;
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

