#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"
#include "TgaImage.h"
#include "Model.h"

constexpr int width = 800;
constexpr int height = 1000;

void line(int x0, int y0, int x1, int y1, TGAImage& image,const TGAColor& color)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
        error2 += derror2;

        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void DrawFilledTriangle(const MathLibrary::vector2& v0,const MathLibrary::vector2& v1,const MathLibrary::vector2& v2,TGAImage&image)
{
    std::vector<MathLibrary::vector2> vertices{ v0,v1,v2 };
    std::qsort(vertices.data(), vertices.size(), sizeof(MathLibrary::vector2),
        [](const void* a, const void* b)
        {
            const MathLibrary::vector2 aa = *static_cast<const MathLibrary::vector2*>(a);
            const MathLibrary::vector2 bb = *static_cast<const MathLibrary::vector2*>(b);
            if (aa.y < bb.y)
            {
                return -1;
            }
            else
            {
                if (aa.y > bb.y)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        });
    float k01 = (vertices[0] - vertices[1]).y / (vertices[0] - vertices[1]).x * 1.0;
    float k02 = (vertices[0] - vertices[2]).y / (vertices[0] - vertices[2]).x * 1.0;
    for (int y = vertices[0].y; y < vertices[1].y; ++y)
    {
        
        int x0 = y * 1.0 / k01 - 1.0 / k01 * vertices[0].y + vertices[0].x;
        int x1 = y * 1.0 / k02 - 1.0 / k02 * vertices[0].y + vertices[0].x;
        line(x0, y, x1, y, image, TGAColor(255, 255, 255, 255));
    }
}

int main(int argc, char** argv)
{
    TGAImage image(width, height, TGAImage::Format::RGB);
    DrawFilledTriangle(MathLibrary::vector2(0, 0), MathLibrary::vector2(200, 50), MathLibrary::vector2(100, 400), image);
    image.write_tga_file("output.tga");
    return 0;
}