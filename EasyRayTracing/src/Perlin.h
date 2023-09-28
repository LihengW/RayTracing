#pragma once
#include "utility.h"

class Perlin {
public:
    Perlin();
    ~Perlin();

    float noise(const glm::vec3& p) const;

    float turb(const glm::vec3& pos, int depth = 7) const;

private:
    static const int point_count = 256;
    glm::vec3* ranvec3;
    int* perm_x;
    int* perm_y;
    int* perm_z;

private:
    static int* perlin_generate_perm();

    static void permute(int* p, int n);

    static float perlin_interp(glm::vec3 c[2][2][2], float u, float v, float w);

};