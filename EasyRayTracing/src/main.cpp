#include "main.h"

int main() {
    int width = 1920;
    int height = 1080;

    Picture* pic = new Picture(width, height, 4);

    float h_offset = 16.0f;
    float v_offset = 9.0f;

    ObjectTable objtable;

    objtable.Add(std::make_shared<Sphere>(glm::vec3( 0.0f, 0.0f, -3.0f ), 2.0f));
    objtable.Add(std::make_shared<Sphere>(glm::vec3(5.0f, 0.0f, -3.0f), 2.0f));
    objtable.Add(std::make_shared<Sphere>(glm::vec3(-5.0f, 0.0f, -3.0f), 2.0f));
    // objtable.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -30.0f, -5.0f), 28.0f));

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y)
        {
            glm::vec3 color{ 0.1f, 0.2f, 0.3f };
            HitRecord rec;
            Ray ray({ h_offset * (-1.0f + 2.0f * x / width), v_offset * (-1.0f + 2.0f * y / height), 0.0f }, { 0.0f, 0.0f, -1.0f});
            if (objtable.hit(ray, rec))
            {
                color = glm::vec3{ 0.6f, 0.1f, 0.3f } * std::max(glm::dot(-ray.GetDir(), rec.normal), 0.0f);
            }
            pic->modify(x, y, color);
        }
    }
    pic->output("assets/output.png");
    
}