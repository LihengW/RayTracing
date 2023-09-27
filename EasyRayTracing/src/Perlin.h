#pragma once
#include "utility.h"

class Perlin {
public:
    Perlin();
    ~Perlin();

    float noise(const glm::vec3& p) const;

private:
    static const int point_count = 256;
    float* ranfloat;
    int* perm_x;
    int* perm_y;
    int* perm_z;

private:
    static int* perlin_generate_perm();

    static void permute(int* p, int n);
};