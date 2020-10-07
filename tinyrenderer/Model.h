#pragma once
#include <vector>
#include <string>
#include "MathLibrary.h"
#include "TgaImage.h"


class Model {
private:
	std::vector<MathLibrary::vector3f> verts_;
	std::vector<std::vector<MathLibrary::vector3i> > faces_; // attention, this vector3i means vertex/uv/normal
	std::vector<MathLibrary::vector3f> norms_;
	std::vector<MathLibrary::vector2f> uv_;
	TGAImage diffusemap_;
	TGAImage normalmap_;
	TGAImage specularmap_;
	void load_texture(std::string filename, const char* suffix, TGAImage& img);
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	MathLibrary::vector3f vert(int i);
	MathLibrary::vector3f vert(int iface, int nthvert);
	MathLibrary::vector2f uv(int iface, int nvert);
	MathLibrary::vector3f norm(int iface, int nvert);
	MathLibrary::vector3f norm(MathLibrary::vector2f uv);
	TGAColor diffuse(MathLibrary::vector2f uv);
	float specular(MathLibrary::vector2f uv);
	std::vector<int> face(int idx);
};