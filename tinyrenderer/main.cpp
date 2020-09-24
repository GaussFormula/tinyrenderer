#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"
#include "TgaImage.h"
#include "Model.h"

const int width = 1024*32;
const int height = 1024*32;
const int depth = 255;

Model* model = nullptr;

MathLibrary::vector3f lightDir(0, 0, -1);
MathLibrary::vector3f camera(0, 0, 3);

MathLibrary::vector3f m2v(const MathLibrary::Matrix& m)
{
    MathLibrary::Matrix& m1=const_cast<MathLibrary::Matrix&>(m);
    return MathLibrary::vector3f(m1[0][0] / m1[3][0], m1[1][0] / m1[3][0], m1[2][0] / m1[3][0]);
}

MathLibrary::Matrix v2m(const MathLibrary::vector3f& v)
{
    MathLibrary::Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.0f;
    return m;
}

MathLibrary::Matrix viewport(int x, int y, int width, int height)
{
    MathLibrary::Matrix m = MathLibrary::Matrix::identity(4);
    m[0][3] = x + width / 2.0f;
    m[1][3] = y + height / 2.0f;
    m[2][3] = depth / 2.0f;

    m[0][0] = width / 2.0f;
    m[1][1] = height / 2.0f;
    m[2][2] = depth / 2.0f;
    return m;
}

void triangle(std::vector<MathLibrary::vector3i> points, std::vector<MathLibrary::vector2i> uvs,
    TGAImage& image, float intensity, std::vector<int>& zbuffer)
{
    if (points[0].y == points[1].y && points[1].y == points[2].y)
    {
        return;
    }
    if (points[0].y > points[1].y)
    {
        std::swap(points[0], points[1]);
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
    for (int i = 0; i < total_height; ++i)
    {
        bool second_half = i > points[1].y - points[0].y || points[1].y == points[0].y;
        int segement_height = second_half ? points[2].y - points[1].y : points[1].y - points[0].y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? points[1].y - points[0].y : 0)) / segement_height;
        MathLibrary::vector3i A = points[0] + MathLibrary::vector3f(points[2] - points[0]) * alpha;
        MathLibrary::vector3i B = second_half ? points[1] + MathLibrary::vector3f(points[2] - points[1]) * beta :
            points[0] + MathLibrary::vector3f(points[1] - points[0]) * beta;
        MathLibrary::vector2i uvA = uvs[0] + (uvs[2] - uvs[0]) * alpha;
        MathLibrary::vector2i uvB = second_half ? uvs[1] + (uvs[2] - uvs[1]) * beta :
            uvs[0] + (uvs[1] - uvs[0]) * beta;
        if (A.x > B.x)
        {
            std::swap(A, B);
            std::swap(uvA, uvB);
        }
        for (int j = A.x; j <= B.x; ++j)
        {
            float phi = B.x == A.x ? 1.0f : (float)(j - A.x) / (float)(B.x - A.x);
            MathLibrary::vector3i P = MathLibrary::vector3f(A) + MathLibrary::vector3f(B - A) * phi;
            MathLibrary::vector2i uvP = uvA + (uvB - uvA) * phi;
            size_t idx = (size_t)P.x + (size_t)P.y * width;
            if (zbuffer[idx] < P.z)
            {
                zbuffer[idx] = P.z;
                TGAColor color = model->diffuse(uvP);
                image.set(P.x, P.y, TGAColor(color.r * intensity, color.g * intensity, color.b * intensity, 255));
            }
        }
    }
}

int main()
{
    model = new Model("african_head.obj");
    //int* zbuffer = new int[(size_t)width * (size_t)height];
    std::vector<int> zbuffer;
    zbuffer.resize((size_t)width * (size_t)height, std::numeric_limits<int>::min());
    /*for (size_t i = 0; i < zbuffer.size(); ++i)
    {
        zbuffer[i] = std::numeric_limits<int>::min();
    }*/
    MathLibrary::Matrix Projection = MathLibrary::Matrix::identity(4);
    MathLibrary::Matrix Viewport = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
    Projection[3][2] = -1.0f / camera.z;
    //memset(zbuffer, std::numeric_limits<int>::min(), sizeof(int) * (size_t)width * (size_t)height);
    
    

    TGAImage image(width, height, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); ++i)
    {
        std::vector<int> face = model->face(i);
        std::vector<MathLibrary::vector3i> screen_coords;
        MathLibrary::vector3f world_coords[3];

        for (int j = 0; j < 3; ++j)
        {
            MathLibrary::vector3f v = model->vert(face[j]);
            screen_coords.push_back(m2v(Viewport * Projection * v2m(v)));
            world_coords[j] = v;
        }

        MathLibrary::vector3f n = MathLibrary::cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * lightDir;
        if (intensity > 0)
        {
            std::vector<MathLibrary::vector2i> uv;
            for (int k = 0; k < 3; ++k)
            {
                uv.push_back(model->uv(i, k));
            }
            triangle(screen_coords, uv, image, intensity, zbuffer);
        }
        std::cout << (float)i / model->nfaces() * 100 << "%" << std::endl;
    }
    image.flip_vertically();
    image.write_tga_file("output.tga");
    //delete[] zbuffer;
    delete model;
    return 0;
}