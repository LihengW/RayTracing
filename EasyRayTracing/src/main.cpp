#include "main.h"

// Scene & Rendering Configurations

static int width = 600;
static float aspectRatio = 1.0f;
static int height = (int)(width * aspectRatio);
static int channel = 4;

static int max_depth = 50;
static int simple_per_pixel = 10;

// Scenes

std::shared_ptr<BVHnode> SphereScene(Camera& _camera, std::shared_ptr<BVHnode>& _scene, ObjectTable& _light)
{
    _camera = Camera({ 0.0f, 3.5f, 0.0f }, { 0.0f, 3.4f, -1.0f }, { 0.0f, 1.0f, 0.0f }, (float)width, 16.0f / 9.0f, 90, 12.0f, 0.005f);

    ObjectTable objtable;
    ObjectTable light;

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

std::shared_ptr<BVHnode> QuadScene(Camera& _camera, std::shared_ptr<BVHnode>& _scene, ObjectTable& _light)
{
    _camera = Camera({ 0.0f, 3.5f, 0.0f }, { 0.0f, 3.4f, -1.0f }, { 0.0f, 1.0f, 0.0f }, (float)width, 16.0f / 9.0f, 90, 12.0f, 0.005f);

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

std::shared_ptr<BVHnode> CornellBox(Camera& _camera, std::shared_ptr<BVHnode>& _scene, ObjectTable& _light)
{
    _camera = Camera({ 278.0f, 278.0f, -800.0f }, { 278.0f, 278.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        (float)width, 1.0f, 
        40, 12.0f, 0.0f);

    auto red =   std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(glm::vec3(.12, .65, .15));
    auto light = std::make_shared<Emit>(white, glm::vec3(25, 25, 25));

    ObjectTable objtable;

    // outer box
    //light
    _light.Add(std::make_shared<Quad>(glm::vec3(343, 554, 300), glm::vec3(-130, 0, 0), glm::vec3(0, 0, -105), light));
    objtable.Add(std::make_shared<Quad>(glm::vec3(343, 554, 300), glm::vec3(-130, 0, 0), glm::vec3(0, 0, -105), light));

    objtable.Add(std::make_shared<Quad>(glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), glm::vec3(0, 555, 0), green));
    objtable.Add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), red));
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

    _scene = std::make_shared<BVHnode>(objtable);

    return _scene;
}


std::shared_ptr<BVHnode> EmitTestScene(Camera& camera)
{
    camera = Camera({ 278.0f, 278.0f, -800.0f }, { 278.0f, 278.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        (float)width, 1.0f,
        40, 12.0f, 0.0f);

    camera.sample_per_pixel = 20;

    auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
    auto light = std::make_shared<Emit>(white, glm::vec3(10, 10, 10));
    auto Glass = std::make_shared<Dielectric>(glm::vec3{ 0.95f }, 1.1f, 0.02f);

    ObjectTable objtable;

    // objtable.Add(std::make_shared<Quad>(glm::vec3(343, 554, 300), glm::vec3(-130, 0, 0), glm::vec3(0, 0, -105), light));
    objtable.Add(std::make_shared<Sphere>(light, glm::vec3(343, 400, 300), 50));
    objtable.Add(std::make_shared<Sphere>(Glass, glm::vec3(343, 400, 300), 80));

    // outer box
    objtable.Add(std::make_shared<Quad>(glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), glm::vec3(0, 555, 0), white));
    objtable.Add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), white));

    objtable.Add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), white));
    objtable.Add(std::make_shared<Quad>(glm::vec3(555, 555, 555), glm::vec3(0, 0, -555), glm::vec3(-555, 0, 0), white));// top
    objtable.Add(std::make_shared<Quad>(glm::vec3(0, 0, 555), glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), white)); // back

    std::shared_ptr<Box> box2 = std::make_shared<Box>(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), white);
    box2->rotateY(-45.0f);
    box2->translate(glm::vec3(130, 0, 65));
    objtable.Add(box2);

    std::shared_ptr<BVHnode> bvh = std::make_shared<BVHnode>(objtable);
    return bvh;
}

// rendering main function

int main() {

    Camera camera;
    std::shared_ptr<BVHnode> scene;
    ObjectTable lights;

    auto res = CornellBox(camera, scene, lights);

    Renderer renderer(width, height, channel, camera, scene, lights);
    
    renderer.SetShadingDepth(max_depth);
    renderer.SetSampleperPixel(simple_per_pixel);

    renderer.Render();
}