#pragma once
#include "TgaImage.h"
#include "MathLibrary.h"

MathLibrary::Matrix4x4f viewport(int x, int y, int width, int height);
MathLibrary::Matrix4x4f projection(float coeff = 0.0f);
MathLibrary::Matrix4x4f lookat(MathLibrary::vector3f eye, MathLibrary::vector3f center, MathLibrary::vector3f up);

class IShader
{
public:
    virtual ~IShader();
    virtual MathLibrary::vector4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(MathLibrary::vector3f bar, TGAColor& color) = 0;
};

void triangle(std::vector<MathLibrary::vector4f> points, IShader&shader, TGAImage& image, TGAImage& zbuffer);