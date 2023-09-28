#pragma once

#include "utility.h"
#include "Perlin.h"

class Texture {
public:
	virtual ~Texture() = default;
	virtual glm::vec3 GetValue(float u, float v, const glm::vec3& p) const = 0;
};

// 2D Tex

class SolidColorTex : public Texture
{
public:
	SolidColorTex(glm::vec3 color) : m_Color(color) {}

	virtual glm::vec3 GetValue(float u, float v, const glm::vec3& p) const override;

private:
	glm::vec3 m_Color;
};

class Texture2D : public Texture
{
public:
    Texture2D(const std::string& path);

    void GetPixelData(int x, int y, glm::vec3& rgb) const;
    void GetPixelData(int x, int y, glm::vec4& rgba) const;

    virtual glm::vec3 GetValue(float u, float v, const glm::vec3& p) const override;

private:
    std::string m_Path;
    int m_Width, m_Height, m_Channel;
    unsigned char* m_Data;
};


// 3D Tex

class PerlinTex : public Texture
{
public:
    PerlinTex() : scale(1.0f) {}
    virtual glm::vec3 GetValue(float u, float v, const glm::vec3& p) const;
    void setscale(float _scale);

private:
    Perlin perlin_noise;
    float scale;
};

class CheckerTex : public Texture
{
	// This is a Tex for exhibition and in spatial
public:
    CheckerTex(float _scale, std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd)
        : inv_scale(1.0f / _scale), even(_even), odd(_odd) {}

    CheckerTex(float _scale, glm::vec3 c1, glm::vec3 c2)
        : inv_scale(1.0f / _scale),
        even(std::make_shared<SolidColorTex>(c1)),
        odd(std::make_shared<SolidColorTex>(c2))
    {}

    virtual glm::vec3 GetValue(float u, float v, const glm::vec3& p) const override;

private:
    float inv_scale;
    std::shared_ptr<Texture> even;
    std::shared_ptr<Texture> odd;
};