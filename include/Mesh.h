#ifndef MESH_H
#define MESH_H


#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <math.h>
#include <list>

#include <Object.h>
#include <Vector.h>

using namespace std;


class TriangleIndices {
public:
	TriangleIndices(int vtxi = -1, int vtxj = -1, int vtxk = -1, int ni = -1, int nj = -1, int nk = -1, int uvi = -1, int uvj = -1, int uvk = -1) : vtxi(vtxi), vtxj(vtxj), vtxk(vtxk), uvi(uvi), uvj(uvj), uvk(uvk), ni(ni), nj(nj), nk(nk) {
	};
	int vtxi, vtxj, vtxk;
	int uvi, uvj, uvk;
	int ni, nj, nk;
	int faceGroup;
};

class BBox {
public:
    BBox(){};
    BBox(const Vector& p_min, const Vector& p_max) : P_min(p_min), P_max(p_max){};
    ~BBox(){};


    bool intersection(const Ray& r) const;

    Vector P_min, P_max;
};

class BVH
{
public:
    BVH(){};
    ~BVH(){};

    uint i0, i1;
    BBox bbox;
    BVH *l_child, *r_child;
};


class Mesh : public Object {
public:
	Mesh() {};
	Mesh(const char* obj, double scaling, const Vector& offset);

	void readOBJ(const char* obj);
	void add_texture(const char* filename);

	Vector getNormal(uint tri_id);
	void setNormals();

	BBox buildBBox(uint tri_0, uint tri_1);
	void buildBVH(BVH*node, uint i0, uint i1);

	BVH bvh;


	virtual bool intersection(const Ray& r, Vector& P, Vector& N, float& t, Vector& color) const;
	bool triIntersection(uint tri_id, const Ray& r, Vector& P, Vector& N, float& t, double& alpha, double& beta, double& gamma) const;


	std::vector<TriangleIndices> indices;
	std::vector<Vector> tri_normals;
	std::vector<Vector> vertices;
	std::vector<Vector> normals;
	std::vector<Vector> uvs; // Vector en 3D mais on n'utilise que 2 composantes
	std::vector<Vector> vertexcolors;

	BBox bbox;

	std::vector<std::vector<unsigned char> > textures;
	std::vector<int> w, h;
};


#endif // MESH_H
