#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

glm::vec3 SolidColorTex::GetValue(float u, float v, const glm::vec3& p) const
{
    return m_Color;
}

glm::vec3 CheckerTex::GetValue(float u, float v, const glm::vec3& p) const
{
    auto xInteger = static_cast<int>(std::floor(inv_scale * p.x));
    auto yInteger = static_cast<int>(std::floor(inv_scale * p.y));
    auto zInteger = static_cast<int>(std::floor(inv_scale * p.z));

    bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

    return isEven ? even->GetValue(u, v, p) : odd->GetValue(u, v, p);
}

Texture2D::Texture2D(const std::string& path)
{
    stbi_set_flip_vertically_on_load(1);
    m_Data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channel, 0);
}

void Texture2D::GetPixelData(int x, int y, glm::vec3& rgb) const
{
    int index = m_Channel * (y*(m_Width)+x);
    rgb.r = (unsigned int)m_Data[index];
    rgb.g = (unsigned int)m_Data[index+1];
    rgb.b = (unsigned int)m_Data[index+2];
}

void Texture2D::GetPixelData(int x, int y, glm::vec4& rgba) const
{
    int index = m_Channel * (y * (m_Width)+x);
    rgba.r = (unsigned int)m_Data[index];
    rgba.g = (unsigned int)m_Data[index + 1];
    rgba.b = (unsigned int)m_Data[index + 2];
    if (m_Channel == 4)
    {
        rgba.a = (unsigned int)m_Data[index + 3];
    }
    else
    {
        rgba.a = 1.0f;
    }
}

glm::vec3 Texture2D::GetValue(float u, float v, const glm::vec3& p) const
{
    glm::vec3 color{ 0.0f };
    GetPixelData((int)(u / 1.0f * m_Width), (int)(v / 1.0f * m_Height), color);
    color = color / 255.0f;
    return color;
}
