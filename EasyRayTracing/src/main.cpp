#include "main.h"

static int width = 192 * 10;
static int height = 108 * 10;
static int sample_per_pixel = 20;
static int max_depth = 5;

glm::vec3 shading(Ray ray, HitRecord& rec, int depth, ObjectTable& objtable)
{
    if (depth >= max_depth) return glm::vec3{ 0.0f };

    if (objtable.hit(ray, rec, 0.0000001f))  // third para use to privent detect themselves (shadow ance)
    {
        if (rec.front_face)
        {
            Ray next_ray;
            glm::vec3 attenuation{0.0f};
            if (rec.mat_ptr->scatter(ray, rec, attenuation, next_ray))
            {

                return attenuation * pow(1.0f / 2.0f, (float)depth) * shading(next_ray, rec, depth + 1, objtable);
            }
            else
            {
                return glm::vec3{ 0.0f };
            }
        }
        else
        {
            return glm::vec3{ 0.0f };
        }
    }
    else
    {
        float a = ray.GetDir().y / 2.0f + 0.5f;
        return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f); // ambient light
    }
}



int main() {
    Picture* pic = new Picture(width, height, 4);

    float h_offset = 16.0f;
    float v_offset = 9.0f;

    ObjectTable objtable;

    Camera camera({ 0.0f, 0.0f, 0.0f }, h_offset, v_offset);

    std::shared_ptr<Lambertian> diffuse_blue = std::make_shared<Lambertian>(glm::vec3{ 0.1f, 0.2f, 0.9f });
    std::shared_ptr<Lambertian> diffuse_org = std::make_shared<Lambertian>(glm::vec3{ 0.8f, 0.8f, 0.3f });
    std::shared_ptr<Metal> metal_red = std::make_shared<Metal>(glm::vec3{ 0.8f, 0.2f, 0.3f });
    std::shared_ptr<Metal> metal_pure = std::make_shared<Metal>(glm::vec3{ 1.0f, 1.0f, 1.0f }, 0.05f);

    objtable.Add(std::make_shared<Sphere>(metal_pure, glm::vec3(0.0f, 0.0f, -8.0f), 6.0f));
    objtable.Add(std::make_shared<Sphere>(metal_red, glm::vec3(5.0f, 0.0f, -2.0f), 1.0f));
    objtable.Add(std::make_shared<Sphere>(diffuse_blue, glm::vec3(-7.0f, 0.0f, -3.0f), 2.0f));
    objtable.Add(std::make_shared<Sphere>(diffuse_org, glm::vec3(-0.0f, -30.0f, -15.0f), 25.0f));
    // objtable.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -30.0f, -5.0f), 28.0f));

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y)
        {
            glm::vec3 colorbuffer(0.0f);
            for (int k = 0; k < sample_per_pixel; k++)
            {
                HitRecord rec;
                Ray ray = camera.GetOrthoRay(-1.0f + 2.0f * (x + Utility::RandomFloat()) / width, -1.0f + 2.0f * (y + Utility::RandomFloat()) / height);
                colorbuffer += shading(ray, rec, 0, objtable);
            }

            pic->modify(x, y, colorbuffer / (float)sample_per_pixel);
        }
    }
    pic->output("assets/output.png");
    
}