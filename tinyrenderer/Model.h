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
	void load_texture(std::string filename, const char* suffix, TGAImage& img);
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	MathLibrary::vector3f vert(int i);
	MathLibrary::vector2i uv(int iface, int nvert);
	TGAColor diffuse(MathLibrary::vector2i uv);
	std::vector<int> face(int idx);
};