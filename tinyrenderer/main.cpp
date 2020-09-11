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
    if (v0.y == v1.y == v2.y)
    {
        return;
    }
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
    int total_height = vertices[2].y - vertices[0].y;
    int height01 = (vertices[1] - vertices[0]).y;
    int height12 = (vertices[2] - vertices[1]).y;
    for (int y = 0; y < total_height; ++y)
    {
        bool second_half = y > height01 || height01 == 0;
        int segement_height = second_half ? height12 : height01;
        float alpha = (float)y / total_height;
        float beta = (float)(y - (second_half ? height01 : 0)) / segement_height;

        MathLibrary::vector2 A = vertices[0] + alpha * (vertices[2] - vertices[0]);
        MathLibrary::vector2 B = second_half ? 
            vertices[1] + beta * (vertices[2] - vertices[1]) : vertices[0] + beta * (vertices[1] - vertices[0]);
        line(A.x, y+vertices[0].y, B.x, y + vertices[0].y, image, TGAColor(255, 255, 255, 255));
        std::cout << A.x << " " << B.x << std::endl;
    }
}

MathLibrary::vector3 barycentric(MathLibrary::vector2* points, MathLibrary::vector2& p)
{
    MathLibrary::vector3 u = MathLibrary::cross(
        MathLibrary::vector3(points[2][0] - points[0][0], points[1][0] - points[0][0], points[0][0] - p[0]),
        MathLibrary::vector3(points[2][1] - points[0][1], points[1][1] - points[0][1], points[0][1] - p[1])
    );
    if (std::abs(u[2]) < 1)
    {
        return MathLibrary::vector3(-1, 1, 1);
    }
    return MathLibrary::vector3(1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void triangle(MathLibrary::vector2* points, TGAImage& image,const TGAColor& color)
{
    MathLibrary::vector2 bboxmin(image.get_width() - 1.0, image.get_height() - 1.0);
    MathLibrary::vector2 bboxmax(0.0, 0.0);
    MathLibrary::vector2 clamp(bboxmin);

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            bboxmin[j] = std::fmax(0, std::fmin(bboxmin[j], points[i][j]));
            bboxmax[j] = std::fmin(clamp[j], std::fmax(bboxmax[j], points[i][j]));
        }
    }
    MathLibrary::vector2 p;
    for (p.x = bboxmin.x; p.x <= bboxmax.x; ++p.x)
    {
        for (p.y = bboxmin.y; p.y <= bboxmax.y; ++p.y)
        {
            MathLibrary::vector3 bc_screen = barycentric(points, p);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
            {
                continue;
            }
            image.set(p.x, p.y, color);
        }
    }
}

int main(int argc, char** argv)
{
    TGAImage image(width, height, TGAImage::Format::RGB);
    MathLibrary::vector2 points[3]{ MathLibrary::vector2(0, 0), MathLibrary::vector2(400, 50), MathLibrary::vector2(100, 400) };
    triangle(points, image,TGAColor(255,255,255,255));
    image.write_tga_file("output.tga");
    return 0;
}