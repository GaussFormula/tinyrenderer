#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"
#include "TgaImage.h"
#include "Model.h"

constexpr int width = 1000;
constexpr int height = 1280;
constexpr int depth = 255;
static std::uint64_t count = 0;
Model* model = nullptr;
int* zbuffer = nullptr;

MathLibrary::vector3f light_dir(0, 0, -1);
MathLibrary::vector3f camera(0, 0, 3);

MathLibrary::vector3f m2v(const MathLibrary::Matrix<4, 1, float>& m)
{
    return MathLibrary::vector3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

MathLibrary::Matrix<4, 1, float> v2m(MathLibrary::vector3f& v)
{
    MathLibrary::Matrix<4, 1, float> m;
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.0f;
    return m;
}

MathLibrary::Matrix<4, 4, float> viewport(int x, int y, int width, int height)
{
    Matrix<4, 4, float> m = MathLibrary::Matrix<4, 4, float>::identity();
    m[0][3] = x + width / 2.0f;
    m[1][3] = y + height / 2.0f;
    m[2][3] = depth / 2.0f;

    m[0][0] = width / 2.f;
    m[1][1] = height / 2.0f;
    m[2][2] = depth / 2.0f;

    return m;
}

void triangle(MathLibrary::vector3i* points,MathLibrary::vector2i* uvs,
    TGAImage&image,float intensity,int* zbuffer)
{
    if (points[0].y == points[1].y && points[1].y == points[2].y)
    {
        //count++;
        return;
    }
    if (points[0].y > points[1].y)
    {
        std::swap(points[0], points[2]);
        std::swap(uvs[0], uvs[1]);
    }
    if (points[0].y > points[2].y)
    {
        std::swap(points[0], points[2]);
        std::swap(uvs[0], uvs[2]);
    }
    if (points[1].y > points[2].y)
    {
        std::swap(points[1], points[2]);
        std::swap(uvs[1], uvs[2]);
    }
    int total_height = points[2].y - points[0].y;

    for (int y = 0; y < total_height; ++y)
    {
        bool second_half = y > points[1].y - points[0].y || points[0].y == points[1].y;
        int current_segment_height = second_half ? points[2].y - points[1].y : points[1].y - points[0].y;
        float alpha = float(y) / total_height;
        float beta = (float)(y - (second_half ? points[1].y - points[0].y : 0)) / current_segment_height;
        MathLibrary::vector3i A = points[0] + alpha * (points[2] - points[0]);
        MathLibrary::vector3i B = second_half ? points[1] + beta * (points[2] - points[1]) : points[0] + beta * (points[1] - points[0]);

        MathLibrary::vector2i uvA = uvs[0] + alpha * (uvs[2] - uvs[0]);
        MathLibrary::vector2i uvB = second_half ? uvs[1] + beta * (uvs[2] - uvs[1]) : uvs[0] + beta * (uvs[1] - uvs[0]);

        if (A.x > B.x)
        {
            std::swap(A, B);
            std::swap(uvA, uvB);
        }

        for (int x = A.x; x <= B.x; ++x)
        {
            float phi = (B.x == A.x) ? 1.0f : float(x - A.x) / (float)(B.x - A.x);
            MathLibrary::vector3f ret = A + phi*(B-A);
            MathLibrary::vector2i uvP = uvA + phi * (uvB - uvA);
            int idx = ret.x + ret.y * width;
            if (zbuffer[idx] < ret.z)
            {
                zbuffer[idx] = ret.z;
                TGAColor color = model->diffuse(MathLibrary::vector2f(uvP.x, uvP.y));
                if (intensity > 0)
                {
                    image.set(ret.x, y + points[0].y, TGAColor(color[2] * intensity, color[1] * intensity, color[0] * intensity));
                }
                else
                {
                    image.set(ret.x, y + points[0].y, TGAColor(255,0,0,255));
                }
                //count++;
            }
            else
            {
                //image.set(ret.x, y + points[0].y, TGAColor(0, 255, 0, 255));
            }
        }
    }
}


int main(int argc, char** argv)
{
    model = new Model("african_head.obj");
    zbuffer = new int[width * height];
    memset(zbuffer, std::numeric_limits<int>::min(), sizeof(int) * width * height);

    MathLibrary::Matrix<4, 4, float> Projection = MathLibrary::Matrix<4, 4, float>::identity();
    MathLibrary::Matrix<4, 4, float> ViewPort = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
    Projection[3][2] = -1.f / camera.z;

    TGAImage image(width, height, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); ++i)
    {
        MathLibrary::vector3i screen_coords[3];
        MathLibrary::vector3f world_coords[3];

        for (int j = 0; j < 3; ++j)
        {
            MathLibrary::vector3f v = model->vert(i, j);
            screen_coords[j] = m2v(ViewPort * Projection * v2m(v));
            world_coords[j] = v;
        }
        MathLibrary::vector3f n = MathLibrary::cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0)
        {
            MathLibrary::vector2i uv[3];
            for (int k = 0; k < 3; ++k)
            {
                uv[k] = MathLibrary::vector2i(model->uv(i, k).x + 0.5f, model->uv(i, k).y + 0.5f);
            }
            triangle(screen_coords, uv, image, intensity, zbuffer);
        }
        else
        {
            MathLibrary::vector2i uv[3];
            for (int k = 0; k < 3; ++k)
            {
                uv[k] = MathLibrary::vector2i(model->uv(i, k).x + 0.5f, model->uv(i, k).y + 0.5f);
            }
            triangle(screen_coords, uv, image, intensity, zbuffer);
        }
    }
    image.write_tga_file("output.tga");
    std::cout << count;
    delete model;
    delete[] zbuffer;
    return 0;
}