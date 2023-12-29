#include "Renderer.h"

Renderer::Renderer(int width, int height, int channel, Camera& camera, std::shared_ptr<BVHnode> world, ObjectTable lights)
	: m_Camera(camera), m_World(world), m_Lights(lights), m_Width(width), m_Height(height), m_Channel(channel)
{
    m_Picture = std::make_unique<Picture>(width, height, channel);
}

Renderer::Renderer(int width, int height, int channel, Camera& camera, ObjectTable world, ObjectTable lights)
	: m_Camera(camera), m_Lights(lights), m_Width(width), m_Height(height), m_Channel(channel)
{
	m_World = std::make_shared<BVHnode>(world);
    m_Picture = std::make_unique<Picture>(width, height, channel);
}

Renderer::Renderer(int width, int height, int channel, Camera& camera, std::shared_ptr<BVHnode> world)
	: m_Camera(camera), m_World(world), m_Width(width), m_Height(height), m_Channel(channel)
{
    m_Picture = std::make_unique<Picture>(width, height, channel);
}

void Renderer::Render()
{
    for (int x = 0; x < m_Width; ++x) {
        for (int y = 0; y < m_Height; ++y)
        {
            glm::vec3 colorbuffer(0.0f);
            for (int k = 0; k < sample_per_pixel; k++)
            {
                HitRecord rec;
                Ray ray = m_Camera.GetPerspectiveRay(-1.0f + 2.0f * (x + Utility::RandomFloat()) / m_Width, -1.0f + 2.0f * (y + Utility::RandomFloat()) / m_Height);
                colorbuffer += shading(ray, rec, 0);
            }
            // output pixel
            m_Picture->modify(x, y, Utility::clamp(colorbuffer / (float)sample_per_pixel, 0.0f, 1.0f));
            std::cout << (y + 1) + x * m_Height << " / " << m_Width * m_Height << " has been rendered!" << "  processing : " << (int)(((y + 1) + x * m_Height) * 100 / (m_Width * m_Height)) << "%" << std::endl;
        }
    }
    m_Picture->output("assets/output/output.png");
}

glm::vec3 Renderer::shading(Ray ray, HitRecord& rec, int depth)
{
    if (depth >= shading_depth)
    {
        rec.reset();
        return glm::vec3{ 0.0f };
    }

    if (m_World->Hit(ray, { 0.000001f, Utility::FLOAT_MAX }, rec))  // third para use to privent detect themselves (shadow ance)
    {
        glm::vec3 attenuation{ 0.0f };
        float pdf_val;
        Ray scatteredray;

        glm::vec3 emit = rec.mat_ptr->emitted(rec);

        if (rec.mat_ptr->scatter(ray, rec, attenuation, scatteredray, pdf_val))
        {
            // lightpdf
            std::shared_ptr<ObjectPDF> light_pdf = std::make_shared<ObjectPDF>(m_Lights.GetTable()[0], rec.position);

            // cospdf
            std::shared_ptr<CosinePDF> surface_pdf = std::make_shared<CosinePDF>(rec.normal);

            // mixed pdf
            std::shared_ptr<MixturePDF> mixed_pdf = std::make_shared<MixturePDF>(light_pdf, surface_pdf);
            scatteredray = Ray(rec.position, mixed_pdf->generate());
            pdf_val = mixed_pdf->value(scatteredray.GetDir());

            float scattering_pdf = rec.mat_ptr->scatter_pdf(ray, rec, scatteredray);
            rec.reset();

            glm::vec3 scatteredcolor = (attenuation * scattering_pdf * shading(scatteredray, rec, depth + 1));
            if (glm::dot(scatteredcolor, scatteredcolor) > 0 && pdf_val > 0)
            {
                scatteredcolor = scatteredcolor / pdf_val;
            }
            return emit + scatteredcolor;
            // return attenuation * pow(1.0f / 2.0f, (float)depth) * shading(next_ray, rec, depth + 1, objtable);
        }
        else
        {
            rec.reset();
            return emit;
        }
    }
    else
    {
        rec.reset();
        return m_Camera.GetBackground();
    }
}
