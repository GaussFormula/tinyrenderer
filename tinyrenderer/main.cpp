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

        for (int x = A.x; x < B.x; ++x)
        {
            float phi = B.x == A.x ? 1.0f : float(x - A.x) / (float)(B.x - A.x);
            MathLibrary::vector3f tempA((float)A.x, (float)A.y, (float)A.z);
            MathLibrary::vector3f tempBA((float)(B - A).x, (float)(B - A).y, (float)(B - A).z);
            MathLibrary::vector3f ret = tempA + tempBA;
            MathLibrary::vector3i p = MathLibrary::vector3i((int)(ret.x+0.5f), (int)(ret.y+0.5f), (int)(ret.z+0.5f));
            MathLibrary::vector2i uvP = uvA + phi * (uvB - uvA);
            int idx = p.x + p.y * width;
            if (zbuffer[idx] < p.z)
            {
                zbuffer[idx] = p.z;
                TGAColor color = model->diffuse(MathLibrary::vector2f(uvP.x, uvP.y));
                image.set(p.x, p.y, color);
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
        MathLibrary::vector3i world_coords[3];

        for (int j = 0; j < 3; ++j)
        {
            MathLibrary::vector3f v = model->vert(i, j);
            MathLibrary::vector3f temp = m2v(ViewPort * Projection * v2m(v));
            screen_coords[j] = MathLibrary::vector3i(temp.x + 0.5f, temp.y + 0.5f, temp.z + 0.5f);
            world_coords[j] = MathLibrary::vector3i(v.x + 0.5f, v.y + 0.5f, v.z + 0.5f);
        }

        MathLibrary::vector<3, int> int_n = cross<int>(screen_coords[2] - screen_coords[0], screen_coords[1] - screen_coords[0]);
        MathLibrary::vector3f n = MathLibrary::vector3f(int_n.x, int_n.y, int_n.z);
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0)
        {
            MathLibrary::vector2i uv[3];
            for (int k = 0; k < 3; ++k)
            {
                uv[k] = MathLibrary::vector2i(model->uv(i, k).x, model->uv(i, k).y);
            }
            triangle(screen_coords, uv, image, intensity, zbuffer);
        }
    }
    image.write_tga_file("output.tga");
    return 0;
}