#include "main.h"

static int width = 1080;
static int height = 1080;
static int max_depth = 50;

glm::vec3 shading(Ray ray, HitRecord& rec, int depth, std::shared_ptr<BVHnode> bvh, Camera camera)
{
    if (depth >= max_depth)
    {
        rec.reset();
        return glm::vec3{ 0.0f };
    }

    if (bvh->Hit(ray, {0.000001f, Utility::FLOAT_MAX}, rec))  // third para use to privent detect themselves (shadow ance)
    {
        Ray next_ray;
        glm::vec3 emit = rec.mat_ptr->emitted(rec.u, rec.v, rec.position);
        glm::vec3 attenuation{ 0.0f };

        if (rec.mat_ptr->scatter(ray, rec, attenuation, next_ray))
        {
            rec.reset();
            return emit + attenuation * shading(next_ray, rec, depth + 1, bvh, camera);
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
        return camera.GetBackground();
    }
}


std::shared_ptr<BVHnode> SphereScene(Camera& camera)
{
    camera = Camera({ 0.0f, 3.5f, 0.0f }, { 0.0f, 3.4f, -1.0f }, { 0.0f, 1.0f, 0.0f }, (float)width, 16.0f / 9.0f, 90, 12.0f, 0.005f);

    ObjectTable objtable;

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
    std::shared_ptr<PerlinTex> perlin_tex = std::make_shared<PerlinTex>();
    metal_pure->BindTex(perlin_tex);

    std::shared_ptr<Texture2D> earth = std::make_shared<Texture2D>("assets/textures/Earth.jpg");
    diffuse_white->BindTex(earth);

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

    // Build BVH Tree
    std::shared_ptr<BVHnode> bvh = std::make_shared<BVHnode>(objtable);
    return bvh;
}

std::shared_ptr<BVHnode> QuadScene(Camera& camera)
{
    camera = Camera({ 0.0f, 3.5f, 0.0f }, { 0.0f, 3.4f, -1.0f }, { 0.0f, 1.0f, 0.0f }, (float)width, 16.0f / 9.0f, 90, 12.0f, 0.005f);

    ObjectTable objtable;

    // Materials
    std::shared_ptr<Lambertian> diffuse_white = std::make_shared<Lambertian>(glm::vec3{ 1.0f, 1.0f, 1.0f });
    std::shared_ptr<Lambertian> diffuse_blue = std::make_shared<Lambertian>(glm::vec3{ 0.1f, 0.2f, 0.9f });
    std::shared_ptr<Lambertian> diffuse_green = std::make_shared<Lambertian>(glm::vec3{ 0.2f, 0.9f, 0.3f });
    std::shared_ptr<Metal> metal_red = std::make_shared<Metal>(glm::vec3{ 0.8f, 0.2f, 0.3f });
    std::shared_ptr<Metal> metal_pure = std::make_shared<Metal>(glm::vec3{ 1.0f, 1.0f, 1.0f }, 0.05f);

    objtable.Add(std::make_shared<Quad>(diffuse_white, glm::vec3(-3, -2, -15), glm::vec3(0, 0, -4), glm::vec3(0, 4, 0)));
    objtable.Add(std::make_shared<Quad>(diffuse_white, glm::vec3(-2, -2, -10), glm::vec3(4, 0, 0), glm::vec3(0, 4, 0)));
    objtable.Add(std::make_shared<Quad>(diffuse_blue, glm::vec3(3, -2, -11), glm::vec3(0, 0, 4), glm::vec3(0, 4, 0)));
    objtable.Add(std::make_shared<Quad>(metal_red, glm::vec3(-2, 3, -11), glm::vec3(4, 0, 0), glm::vec3(0, 0, 4)));
    objtable.Add(std::make_shared<Quad>(metal_pure, glm::vec3(-2, -3, -5),glm::vec3(4, 0, 0),  glm::vec3(0, 0, -4)));

    // Build BVH Tree
    std::shared_ptr<BVHnode> bvh = std::make_shared<BVHnode>(objtable);
    return bvh;

}

std::shared_ptr<BVHnode> LightTestScene(Camera& camera)
{
    camera =  Camera({ 0.0f, 3.5f, 0.0f }, { 0.0f, 3.4f, -1.0f }, { 0.0f, 1.0f, 0.0f }, (float)width, 16.0f / 9.0f, 90, 12.0f, 0.005f);

    ObjectTable objtable;
    std::shared_ptr<Lambertian> diffuse_white = std::make_shared<Lambertian>(glm::vec3{ 1.0f, 1.0f, 1.0f });
    std::shared_ptr<Emit> Lightsource = std::make_shared<Emit>(diffuse_white, glm::vec3{ 1.0f, 1.0f, 0.7f });
    std::shared_ptr<Emit> RedLightsource = std::make_shared<Emit>(diffuse_white, glm::vec3{ 0.8f, 0.3f, 0.2f });
    std::shared_ptr<Lambertian> diffuse_green = std::make_shared<Lambertian>(glm::vec3{ 0.2f, 0.9f, 0.3f });
    std::shared_ptr<Dielectric> glass = std::make_shared<Dielectric>(glm::vec3{ 1.0f, 1.0f, 1.0f }, 1.1f, 0.0f);

    std::shared_ptr<Texture2D> earth = std::make_shared<Texture2D>("assets/textures/Earth.jpg");
    diffuse_white->BindTex(earth);

    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(Lightsource, glm::vec3(0.0f, 6.0f, -15.0f), 6.0f);
    objtable.Add(s1);
    objtable.Add(std::make_shared<Sphere>(diffuse_white, glm::vec3(-0.0f, -91.0f, -20.0f), 90.0f));
    objtable.Add(std::make_shared<Sphere>(glass, glm::vec3(7.0f, 3.0f, -12.0f), 1.5f));
    // objtable.Add(std::make_shared<Sphere>(RedLightsource, glm::vec3(15.0f, 4.0f, -12.0f), 4.0f));
    objtable.Add(std::make_shared<Sphere>(diffuse_green, glm::vec3(-7.0f, 3.5f, -13.0f), 2.0f));

    std::shared_ptr<BVHnode> bvh = std::make_shared<BVHnode>(objtable);
    return bvh;
}

std::shared_ptr<BVHnode> CornellBox(Camera& camera)
{
    camera = Camera({ 278.0f, 278.0f, -800.0f }, { 278.0f, 278.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 
        (float)width, 1.0f, 
        40, 12.0f, 0.0f);

    camera.sample_per_pixel = 50;

    auto red =   std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(glm::vec3(.12, .65, .15));
    auto light = std::make_shared<Emit>(white, glm::vec3(25, 25, 25));

    ObjectTable objtable;

    // outer box
    objtable.Add(std::make_shared<Quad>(glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), glm::vec3(0, 555, 0), green));
    objtable.Add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), red));
    objtable.Add(std::make_shared<Quad>(glm::vec3(343, 554, 300), glm::vec3(-130, 0, 0), glm::vec3(0, 0, -105), light));

    objtable.Add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), white));
    objtable.Add(std::make_shared<Quad>(glm::vec3(555, 555, 555), glm::vec3(0, 0, -555), glm::vec3(-555, 0, 0), white));// top
    objtable.Add(std::make_shared<Quad>(glm::vec3(0, 0, 555), glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), white)); // back

    // inside box
    std::shared_ptr<Box> box1 = std::make_shared<Box>(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), white);
    box1->rotateY(15.0f);
    box1->translate(glm::vec3(265, 0, 295));
    objtable.Add(box1);

    std::shared_ptr<Box> box2 = std::make_shared<Box>(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), white);
    box2->rotateY(-18.0f);
    box2->translate(glm::vec3(130, 0, 65));
    objtable.Add(box2);


    std::shared_ptr<BVHnode> bvh = std::make_shared<BVHnode>(objtable);
    return bvh;
}


int main() {
    Picture* pic = new Picture(width, height, 4);

    Camera camera;

    auto bvh = CornellBox(camera);

    // Shading every pixel
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y)
        {
            glm::vec3 colorbuffer(0.0f);
            for (int k = 0; k < camera.sample_per_pixel; k++)
            {
                HitRecord rec;
                Ray ray = camera.GetPerspectiveRay(-1.0f + 2.0f * (x + Utility::RandomFloat()) / width, -1.0f + 2.0f * (y + Utility::RandomFloat()) / height);
                colorbuffer += shading(ray, rec, 0, bvh, camera);
            }
            // output pixel
            pic->modify(x, y, Utility::clamp(colorbuffer / (float)camera.sample_per_pixel, 0.0f, 1.0f));
            std::cout << (y + 1) + x * height << " / " << width * height << " has been rendered!" << "  processing : " << (float)(((y + 1) + x * height) / (width * height)) * 100.0f << "%" << std::endl;
        }
    }
    pic->output("assets/output/output.png");
    
}