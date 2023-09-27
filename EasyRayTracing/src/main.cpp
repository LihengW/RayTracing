#include "main.h"

static int width = 192 * 10;
static int height = 108 * 10;
static int sample_per_pixel = 20;
static int max_depth = 10;

glm::vec3 shading(Ray ray, HitRecord& rec, int depth, BVHnode& bvh)
{
    if (depth >= max_depth)
    {
        return glm::vec3{ 0.0f };
    }

    if (bvh.Hit(ray, {0.000001f, Utility::FLOAT_MAX}, rec))  // third para use to privent detect themselves (shadow ance)
    {
        Ray next_ray;
        glm::vec3 attenuation{0.0f};
        if (rec.mat_ptr->scatter(ray, rec, attenuation, next_ray))
        {
            rec.reset();
            return attenuation * shading(next_ray, rec, depth + 1, bvh);
            // return attenuation * pow(1.0f / 2.0f, (float)depth) * shading(next_ray, rec, depth + 1, objtable);
        }
        else
        {
            rec.reset();
            return glm::vec3{ 0.0f };
        }
    }
    else
    {
        float a = ray.GetDir().y / 2.0f + 0.5f;
        rec.reset();
        return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f); // ambient light
    }
}



int main() {
    Picture* pic = new Picture(width, height, 4);

    float v_offset = 16.0f;
    float h_offset = 9.0f;

    ObjectTable objtable;

    Camera camera({ 0.0f, 3.5f, 0.0f }, { 0.0f, 3.4f, -1.0f }, {0.0f, 1.0f, 0.0f}, (float)width, v_offset / h_offset, 90, 12.0f, 0.005f);

    // Materials
    std::shared_ptr<Lambertian> diffuse_white = std::make_shared<Lambertian>(glm::vec3{ 1.0f, 1.0f, 1.0f });
    std::shared_ptr<Lambertian> diffuse_blue = std::make_shared<Lambertian>(glm::vec3{ 0.1f, 0.2f, 0.9f });
    std::shared_ptr<Lambertian> diffuse_green = std::make_shared<Lambertian>(glm::vec3{ 0.2f, 0.9f, 0.3f });
    std::shared_ptr<Metal> metal_red = std::make_shared<Metal>(glm::vec3{ 0.8f, 0.2f, 0.3f });
    std::shared_ptr<Metal> metal_pure = std::make_shared<Metal>(glm::vec3{ 1.0f, 1.0f, 1.0f }, 0.05f);
    std::shared_ptr<Dielectric> glass = std::make_shared<Dielectric>(glm::vec3{ 1.0f, 1.0f, 1.0f }, 1.1f, 0.0f);
    std::shared_ptr<Dielectric> glass_green = std::make_shared<Dielectric>(glm::vec3{ 0.0f, 0.9f, 0.9f }, 1.5f, 0.0f);

    // Textures
    std::shared_ptr<CheckerTex> checker_tex = std::make_shared<CheckerTex>(5.00f, glm::vec3{ 0.3f, 0.7f, 0.2f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
    metal_pure->BindTex(checker_tex);

    std::shared_ptr<Texture2D> kita = std::make_shared<Texture2D>("assets/textures/Earth.jpg");
    diffuse_white->BindTex(kita);

    // Objects
    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(glass, glm::vec3(0.0f, 6.0f, -15.0f), 6.0f);
    AABB box = AABB();
    s1->SetBB(box);
    objtable.Add(s1);

    std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(diffuse_white, glm::vec3(0.0f, 10.0f, -100.0f), 70.0f);
    s2->SetUVOffset(0.27f, 0.25f);
    objtable.Add(s2);

    objtable.Add(std::make_shared<Sphere>(diffuse_green, glm::vec3(9.0f, 5.5f, -30.0f), 5.0f));
    // objtable.Add(std::make_shared<Sphere>(metal_pure, glm::vec3(-6.0f, 7.5f, -35.0f), 5.0f)); this one looks wired !

    objtable.Add(std::make_shared<Sphere>(metal_red, glm::vec3(5.0f, 2.0f, -12.0f), 1.0f));
    objtable.Add(std::make_shared<Sphere>(diffuse_blue, glm::vec3(-7.0f, 3.5f, -13.0f), 2.0f));

    objtable.Add(std::make_shared<Sphere>(metal_pure, glm::vec3(-0.0f, -90.0f, -20.0f), 90.0f));
    // objtable.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -30.0f, -5.0f), 28.0f));

    // Create BVH Tree
    BVHnode bvh = BVHnode(objtable);

    // Shading every pixel
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y)
        {
            glm::vec3 colorbuffer(0.0f);
            for (int k = 0; k < sample_per_pixel; k++)
            {
                HitRecord rec;
                Ray ray = camera.GetPerspectiveRay(-1.0f + 2.0f * (x + Utility::RandomFloat()) / width, -1.0f + 2.0f * (y + Utility::RandomFloat()) / height);
                colorbuffer += shading(ray, rec, 0, bvh);
            }
            // output pixel
            pic->modify(x, y, colorbuffer / (float)sample_per_pixel);
        }
    }
    pic->output("assets/output.png");
    
}