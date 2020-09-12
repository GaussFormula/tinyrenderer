#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"
#include "TgaImage.h"
#include "Model.h"

constexpr int width = 10000;
constexpr int height = 12800;
static std::uint64_t count = 0;

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

void DrawFilledTriangle(const MathLibrary::vector2& v0,const MathLibrary::vector2& v1,const MathLibrary::vector2& v2,TGAImage&image,TGAColor color)
{
    if (v0.y == v1.y&&v1.y == v2.y)
    {
        return;
    }
    if (v0.x == v1.x && v1.x == v2.x)
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
        if (A.x > B.x)
        {
            std::swap(A, B);
        }
        for (int j = A.x; j <= B.x; ++j)
        {
            image.set(j, vertices[0].y + y, color);
        }
    }
    count++;
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
    return MathLibrary::vector3(u.y / u.z, 1.0f - (u.y + u.x) / u.z,  u.x / u.z);
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
    count++;
}

int main(int argc, char** argv)
{
    TGAImage image(width, height, TGAImage::Format::RGB);
    MathLibrary::vector3 light_dir(0, 0, -1);
    std::unique_ptr<Model> p = std::make_unique<Model>("african_head.obj");
    for (int i = 0; i < p->nfaces(); ++i)
    {
        MathLibrary::vector2 screen_coords[3];
        MathLibrary::vector3 world_coords[3];
        for (int j = 0; j < 3; ++j)
        {
            MathLibrary::vector3 v = p->vert(i, j);
            screen_coords[j] = MathLibrary::vector2((v.x + 1.0) * width / 2, (v.y + 1.0) * height / 2);
            world_coords[j] = v;
        }
        MathLibrary::vector3 n = cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0)
        {
            DrawFilledTriangle(screen_coords[0],screen_coords[1],screen_coords[2],image,TGAColor(intensity*255, intensity * 255, intensity * 255, 255));
            //triangle(screen_coords, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
        }
    }
    image.write_tga_file("output.tga");
    std::cout << count;
    return 0;
}