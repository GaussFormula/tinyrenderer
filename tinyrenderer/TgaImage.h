#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

#pragma pack(push,1)
struct TGA_Header
{
    std::uint8_t    idLength{};
    std::uint8_t    colorMapType{};
    std::uint8_t    dataTypeCode{};
    std::uint16_t   colorMapOrigin{};
    std::uint16_t   colorMapLength{};
    std::uint8_t    colorMapDepth{};
    std::uint16_t   x_origin{};
    std::uint16_t   y_origin{};
    std::uint16_t   width{};
    std::uint16_t   height{};
    std::uint8_t    bitsPerPixel{};
    std::uint8_t    imageDescriptor{};
};
#pragma pack(pop)

class TGAColor
{
public:
    std::uint8_t bgra[4] = { 0,0,0,0 };
    std::uint8_t bytespp = { 0 };

    TGAColor() = default;
    TGAColor(const std::uint8_t R,const std::uint8_t G,const std::uint8_t B,const std::uint8_t A=255)
        :bgra{B,G,R,A},bytespp(4)
    {}
    TGAColor(const std::uint8_t v)
        :bgra{v,0,0,0},bytespp(1)
    {}

    std::uint8_t& operator[](const int i)
    {
        return bgra[i];
    }

    TGAColor operator*(const double intensity)const
    {
        TGAColor res = *this;
        double clamped = std::fmax(0., std::fmin(intensity, 1.));
        for (int i = 0; i < 4; ++i)
        {
            res.bgra[i] = this->bgra[i] * clamped;
        }
        return res;
    }
};