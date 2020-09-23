#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"
#include "TgaImage.h"
#include "Model.h"

const int width = 800;
const int height = 800;
const int depth = 255;

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

int main()
{
    MathLibrary::Matrix m(4, 1);
    m[0][0] = 1.0f;
    m[1][0] = 2.0f;
    m[2][0] = 2.0f;
    m[3][0] = 4.0f;
    MathLibrary::Matrix m1(4,1);
    m1 = m;
    std::cout << m1;
    return 0;
}