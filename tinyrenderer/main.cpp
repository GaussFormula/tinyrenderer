#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"
#include "TgaImage.h"

constexpr int width = 800;
constexpr int height = 600;

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
    bool steep = false;
    int dx = std::abs(x0 - x1);
    int dy = std::abs(y0 - y1);
    if (dx < dy)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    float derror = std::abs(dy / (float)dx);
    float error = 0;
    int y = y0;
    for (int x = x0; x < x1; ++x)
    {
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        error += derror;
        if (error > 0.5)
        {
            y += (y1 > y0 ? 1 : -1);
            error -= 1.0;
        }
    }
}

int main(int argc, char** argv)
{
    TGAImage image(width, height, TGAImage::Format::RGB);
    line(13, 20, 80, 40, image, TGAColor(255,0,0,255));
    line(120, 113, 40, 80, image, TGAColor(255,255,255,255));
    line(80, 40, 13, 20, image, TGAColor(255, 255, 255, 255));
    image.flip_vertically();
    image.write_tga_file("output.tga",false,false);
    return 0;
}