#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"
#include "TgaImage.h"
#include "Model.h"
#include "our_gl.h"

const int width = 1024;
const int height = 1024;
const int depth = 255;

Model* model = nullptr;

MathLibrary::vector3f lightDir(0, 0, -1);
MathLibrary::vector3f eye(2, 2, 2);
MathLibrary::vector3f center(0, 0, 0);
MathLibrary::vector3f up(0, 1, 0);

MathLibrary::vector3f m2v(const MathLibrary::Matrix<4,1,float>& m)
{
    MathLibrary::Matrix<4, 1, float>& m1 = const_cast<MathLibrary::Matrix<4, 1, float>&>(m);
    return MathLibrary::vector3f(m1[0][0] / m1[3][0], m1[1][0] / m1[3][0], m1[2][0] / m1[3][0]);
}

MathLibrary::Matrix<4,1,float> v2m(const MathLibrary::vector3f& v)
{
    MathLibrary::Matrix<4,1,float> m;
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.0f;
    return m;
}

class GouraudShader :public IShader
{
private:
    MathLibrary::Matrix4x4f Viewport = MathLibrary::Matrix4x4f::identity();
    MathLibrary::Matrix4x4f Projection = MathLibrary::Matrix4x4f::identity();
    MathLibrary::Matrix4x4f ModelView = MathLibrary::Matrix4x4f::identity();

public:
    MathLibrary::vector3f varying_intensity;// written by vertex shader, read by fragment shader.
    virtual MathLibrary::vector4f vertex(int iface, int nthvert)
    {
        MathLibrary::vector4f gl_vertex = MathLibrary::embed<4>(model->vert(iface, nthvert));
        gl_vertex = Viewport * Projection * ModelView * gl_vertex;
        varying_intensity[nthvert] = std::fmax(0.0f, model->norm(iface, nthvert) * lightDir);
        return gl_vertex;
    }

    virtual bool fragment(MathLibrary::vector3f bar, TGAColor& color)
    {
        float intensity = varying_intensity * bar;
        color = TGAColor(255, 255, 255) * intensity;
        return false;
    }

    GouraudShader(MathLibrary::Matrix4x4f& Viewport_,MathLibrary::Matrix4x4f& Projection_,MathLibrary::Matrix4x4f& ModelView_)
        :Viewport(Viewport_),Projection(Projection_),ModelView(ModelView_)
    {}
};

int main()
{
    model = new Model("african_head.obj");

    MathLibrary::Matrix4x4f ModelView = lookat(eye, center, up);
    MathLibrary::Matrix4x4f Projection = projection(-1.0f / (eye - center).norm());
    MathLibrary::Matrix4x4f Viewport = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
    lightDir.normalize();

    GouraudShader shader(Viewport, Projection, ModelView);
    /*MathLibrary::Matrix4x4f z = (Viewport * Projection * ModelView);
    std::cout << Viewport;*/


    TGAImage image(width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);
    for (int i = 0; i < model->nfaces(); ++i)
    {
        std::vector<MathLibrary::vector4f>screen_coords;
        for (int j = 0; j < 3; ++j)
        {
            screen_coords.push_back(shader.vertex(i, j));
        }
        triangle(screen_coords, shader, image, zbuffer);
        std::cout << (float)i / model->nfaces() * 100 << "%" << std::endl;
    }
    image.flip_vertically();
    image.write_tga_file("output.tga");
    zbuffer.write_tga_file("zbuffer.tga");
    //delete[] zbuffer;
    delete model;
    //std::cout << lookat(eye, center, MathLibrary::vector3f(0, 1, 0));
    return 0;
}