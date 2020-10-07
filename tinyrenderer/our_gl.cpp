#include <cmath>
#include <limits>
#include <cstdlib>
#include "our_gl.h"

IShader::~IShader()
{}

MathLibrary::Matrix4x4f viewport(int x, int y, int width, int height)
{
    MathLibrary::Matrix4x4f m = MathLibrary::Matrix4x4f::identity();
    m[0][3] = x + width / 2.0f;
    m[1][3] = y + height / 2.0f;
    m[2][3] = 255.0f / 2.0f;

    m[0][0] = width / 2.0f;
    m[1][1] = height / 2.0f;
    m[2][2] = 255.0f / 2.0f;
    return m;
}

MathLibrary::Matrix4x4f lookat(MathLibrary::vector3f eye, MathLibrary::vector3f center, MathLibrary::vector3f up)
{
    MathLibrary::vector3f z = (eye - center).normalize();
    MathLibrary::vector3f x = (MathLibrary::cross(up, z)).normalize();
    MathLibrary::vector3f y = MathLibrary::cross(z, x).normalize();

    MathLibrary::Matrix4x4f res = MathLibrary::Matrix4x4f::identity();
    MathLibrary::Matrix4x4f temp = MathLibrary::Matrix4x4f::identity();
    for (int i = 0; i < 3; ++i)
    {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        temp[i][3] = -eye[i];
    }
    return res * temp;
}

MathLibrary::vector3f barycentric(const MathLibrary::vector2f& A,const MathLibrary::vector2f& B,const MathLibrary::vector2f& C,const MathLibrary::vector2f& P)
{
    MathLibrary::vector3f s[2];
    for (int i = 0; i < 2; ++i)
    {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    MathLibrary::vector3f u = cross(s[0], s[1]);
    if (std::abs(u[2]) > 1e-2)
    {
        return MathLibrary::vector3f(1.0f - (u.x + u.y), u.y / u.z, u.x / u.z);
    }
    return MathLibrary::vector3f(-1, 1, 1);
}

void triangle(std::vector<MathLibrary::vector4f> points, IShader& shader, TGAImage& image, TGAImage& zbuffer)
{
    MathLibrary::vector2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    MathLibrary::vector2f bboxmax(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            bboxmin[j] = std::fmin(bboxmin[j], points[i][j] / points[i][3]);
            bboxmax[j] = std::fmax(bboxmax[j], points[i][j] / points[i][3]);
        }
    }

    MathLibrary::vector2i P;
    TGAColor color;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; ++P.x)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; ++P.y)
        {
            MathLibrary::vector3f c =
                barycentric(MathLibrary::proj<2>(points[0] / points[0][3]),
                    MathLibrary::proj<2>(points[1] / points[1][3]),
                    MathLibrary::proj<2>(points[2] / points[2][3]),
                    MathLibrary::proj<2>(P));
            float z = MathLibrary::vector3f(points[0][2], points[1][2], points[2][2]) * c;
            float w = MathLibrary::vector3f(points[0][3], points[1][3], points[2][3]) * c;
            int frag_depth = (int)std::fmax(0, std::fmin(255, int(z / w + 0.5f)));
            if (c.x < 0 || c.y < 0 || c.z<0 || zbuffer.get(P.x, P.y)[0]>frag_depth)
            {
                continue;
            }
            bool discard = shader.fragment(c, color);
            if (!discard)
            {
                zbuffer.set(P.x, P.y, TGAColor(frag_depth));
                image.set(P.x, P.y, color);
            }
        }
    }
}

MathLibrary::Matrix4x4f projection(float coeff /* = 0.0f */)
{
    MathLibrary::Matrix4x4f result = MathLibrary::Matrix4x4f::identity();
    result[3][2] = coeff;
    return result;
}