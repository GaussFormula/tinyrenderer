#pragma once
#include <vector>
#include <string>
#include "MathLibrary.h"
#include "TgaImage.h"

using namespace MathLibrary;

class Model {
private:
    std::vector<vector3> verts_;     // array of vertices
    std::vector<vector2> uv_;        // array of tex coords
    std::vector<vector3> norms_;     // array of normal vectors
    std::vector<int> facet_vrt_;
    std::vector<int> facet_tex_;  // indices in the above arrays per triangle
    std::vector<int> facet_nrm_;
    TGAImage diffusemap_;         // diffuse color texture
    TGAImage normalmap_;          // normal map texture
    TGAImage specularmap_;        // specular map texture
    void load_texture(const std::string filename, const std::string suffix, TGAImage& img);
public:
    Model(const std::string filename);
    int nverts() const;
    int nfaces() const;
    vector3 normal(const int iface, const int nthvert) const;  // per triangle corner normal vertex
    vector3 normal(const vector2& uv) const;                      // fetch the normal vector from the normal map texture
    vector3 vert(const int i) const;
    vector3 vert(const int iface, const int nthvert) const;
    vector2 uv(const int iface, const int nthvert) const;
    TGAColor diffuse(const vector2& uv) const;
    double specular(const vector2& uv) const;
};