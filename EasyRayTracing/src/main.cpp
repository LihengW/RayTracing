#include "main.h"

int main() {

    Picture* pic = new Picture(255, 255, 4);
    for (int x = 0; x < 255; ++x) {
        for (int y = 0; y < 255; ++y)
        {
            pic->modify(x, y, { x / 255.0f, y / 255.0f, 0.2f, 1.0f });
        }
    }
    pic->output("assets/output.png");
    
}