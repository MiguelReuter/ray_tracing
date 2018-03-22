#include "Mesh.h"



bool BBox::intersection(const Ray& r) const
{
    double txmin, tymin, tzmin, txmax, tymax, tzmax;
    double t1, t2;
    // intersection plan x / ray
    t1 = (P_min.x - r.getOrigin().x) / r.getDirection().x;
    t2 = (P_max.x - r.getOrigin().x) / r.getDirection().x;
    txmin = min(t1, t2);
    txmax = max(t1, t2);

    // intersection plan y / ray
    t1 = (P_min.y - r.getOrigin().y) / r.getDirection().y;
    t2 = (P_max.y - r.getOrigin().y) / r.getDirection().y;
    tymin = min(t1, t2);
    tymax = max(t1, t2);

    // intersection plan z / ray
    t1 = (P_min.z - r.getOrigin().z) / r.getDirection().z;
    t2 = (P_max.z - r.getOrigin().z) / r.getDirection().z;
    tzmin = min(t1, t2);
    tzmax = max(t1, t2);

    if ( min(min(txmax, tymax), tzmax) <= 0) // box on ray's back
        return false;

    if ( min(min(txmax, tymax), tzmax) - max(max(txmin, tymin), tzmin) > 0 )
        return true;
    return false;
}


Mesh::Mesh(const char* obj, double scaling, const Vector& offset)
{
    readOBJ(obj);
    add_texture("models/Rabbit/Rabbit_D.bmp");

    for (uint i = 0; i < vertices.size(); i++)
        vertices[i] = vertices[i] * scaling + offset;

    bbox = buildBBox(0, indices.size());
    buildBVH(&bvh, 0, indices.size());
}


void Mesh::readOBJ(const char* obj)
{

    char matfile[255];
    char grp[255];

    FILE* f;
    f = fopen(obj, "r");

    std::map<std::string, int> groupNames;
    int curGroup = -1;
    while (!feof(f)) {
        char line[255];
        if (!fgets(line, 255, f)) break;

        std::string linetrim(line);
        linetrim.erase(linetrim.find_last_not_of(" \r\t") + 1);
        strcpy(line, linetrim.c_str());

        if (line[0] == 'u' && line[1] == 's') {
            sscanf(line, "usemtl %[^\n]\n", grp);
            if (groupNames.find(std::string(grp)) != groupNames.end()) {
                curGroup = groupNames[std::string(grp)];
            }
            else {
                curGroup = groupNames.size();
                groupNames[std::string(grp)] = curGroup;
            }
        }
        if (line[0] == 'm' && line[1] == 't' && line[2] == 'l') {
            sscanf(line, "mtllib %[^\n]\n", matfile);
        }
        if (line[0] == 'v' && line[1] == ' ') {
            Vector vec;
            Vector col;
            if (sscanf(line, "v %lf %lf %lf %lf %lf %lf\n", &vec.x, &vec.y, &vec.z, &col.x, &col.y, &col.z) == 6) {
                vertices.push_back(vec);
                vertexcolors.push_back(col);
            }
            else {
                sscanf(line, "v %lf %lf %lf\n", &vec.x, &vec.y, &vec.z);  // helmet
                                                                             //vec[2] = -vec[2]; //car2
                vertices.push_back(vec);
            }
        }
        if (line[0] == 'v' && line[1] == 'n') {
            Vector vec;
            sscanf(line, "vn %lf %lf %lf\n", &vec.x, &vec.y, &vec.z); //girl
            normals.push_back(vec);
        }
        if (line[0] == 'v' && line[1] == 't') {
            Vector vec;
            sscanf(line, "vt %lf %lf\n", &vec.x, &vec.y);
            uvs.push_back(vec);
        }
        if (line[0] == 'f') {
            TriangleIndices t;
            int i0, i1, i2, i3;
            int j0, j1, j2, j3;
            int k0, k1, k2, k3;
            int nn;

            char* consumedline = line + 1;
            int offset;
            t.faceGroup = curGroup;
            nn = sscanf(consumedline, "%u/%u/%u %u/%u/%u %u/%u/%u%n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2, &offset);
            if (nn == 9) {
                if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
                if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
                if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
                if (j0 < 0) t.uvi = uvs.size() + j0; else	t.uvi = j0 - 1;
                if (j1 < 0) t.uvj = uvs.size() + j1; else	t.uvj = j1 - 1;
                if (j2 < 0) t.uvk = uvs.size() + j2; else	t.uvk = j2 - 1;
                if (k0 < 0) t.ni = normals.size() + k0; else	t.ni = k0 - 1;
                if (k1 < 0) t.nj = normals.size() + k1; else	t.nj = k1 - 1;
                if (k2 < 0) t.nk = normals.size() + k2; else	t.nk = k2 - 1;

                indices.push_back(t);
            }
            else {
                nn = sscanf(consumedline, "%u/%u %u/%u %u/%u%n", &i0, &j0, &i1, &j1, &i2, &j2, &offset);
                if (nn == 6) {
                    if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
                    if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
                    if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
                    if (j0 < 0) t.uvi = uvs.size() + j0; else	t.uvi = j0 - 1;
                    if (j1 < 0) t.uvj = uvs.size() + j1; else	t.uvj = j1 - 1;
                    if (j2 < 0) t.uvk = uvs.size() + j2; else	t.uvk = j2 - 1;
                    indices.push_back(t);
                }
                else {
                    nn = sscanf(consumedline, "%u %u %u%n", &i0, &i1, &i2, &offset);
                    if (nn == 3) {
                        if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
                        if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
                        if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
                        indices.push_back(t);
                    }
                    else {
                        nn = sscanf(consumedline, "%u//%u %u//%u %u//%u%n", &i0, &k0, &i1, &k1, &i2, &k2, &offset);
                        if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
                        if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
                        if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
                        if (k0 < 0) t.ni = normals.size() + k0; else	t.ni = k0 - 1;
                        if (k1 < 0) t.nj = normals.size() + k1; else	t.nj = k1 - 1;
                        if (k2 < 0) t.nk = normals.size() + k2; else	t.nk = k2 - 1;
                        indices.push_back(t);
                    }
                }
            }


            consumedline = consumedline + offset;

            while (true) {
                if (consumedline[0] == '\n') break;
                if (consumedline[0] == '\0') break;
                nn = sscanf(consumedline, "%u/%u/%u%n", &i3, &j3, &k3, &offset);
                TriangleIndices t2;
                t2.faceGroup = curGroup;
                if (nn == 3) {
                    if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
                    if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
                    if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
                    if (j0 < 0) t2.uvi = uvs.size() + j0; else	t2.uvi = j0 - 1;
                    if (j2 < 0) t2.uvj = uvs.size() + j2; else	t2.uvj = j2 - 1;
                    if (j3 < 0) t2.uvk = uvs.size() + j3; else	t2.uvk = j3 - 1;
                    if (k0 < 0) t2.ni = normals.size() + k0; else	t2.ni = k0 - 1;
                    if (k2 < 0) t2.nj = normals.size() + k2; else	t2.nj = k2 - 1;
                    if (k3 < 0) t2.nk = normals.size() + k3; else	t2.nk = k3 - 1;
                    indices.push_back(t2);
                    consumedline = consumedline + offset;
                    i2 = i3;
                    j2 = j3;
                    k2 = k3;
                }
                else {
                    nn = sscanf(consumedline, "%u/%u%n", &i3, &j3, &offset);
                    if (nn == 2) {
                        if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
                        if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
                        if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
                        if (j0 < 0) t2.uvi = uvs.size() + j0; else	t2.uvi = j0 - 1;
                        if (j2 < 0) t2.uvj = uvs.size() + j2; else	t2.uvj = j2 - 1;
                        if (j3 < 0) t2.uvk = uvs.size() + j3; else	t2.uvk = j3 - 1;
                        consumedline = consumedline + offset;
                        i2 = i3;
                        j2 = j3;
                        indices.push_back(t2);
                    }
                    else {
                        nn = sscanf(consumedline, "%u//%u%n", &i3, &k3, &offset);
                        if (nn == 2) {
                            if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
                            if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
                            if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
                            if (k0 < 0) t2.ni = normals.size() + k0; else	t2.ni = k0 - 1;
                            if (k2 < 0) t2.nj = normals.size() + k2; else	t2.nj = k2 - 1;
                            if (k3 < 0) t2.nk = normals.size() + k3; else	t2.nk = k3 - 1;
                            consumedline = consumedline + offset;
                            i2 = i3;
                            k2 = k3;
                            indices.push_back(t2);
                        }
                        else {
                            nn = sscanf(consumedline, "%u%n", &i3, &offset);
                            if (nn == 1) {
                                if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
                                if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
                                if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
                                consumedline = consumedline + offset;
                                i2 = i3;
                                indices.push_back(t2);
                            }
                            else {
                                consumedline = consumedline + 1;
                            }
                        }
                    }
                }
            }
        }
    }
    fclose(f);

    setNormals();
}

void Mesh::add_texture(const char* filename)
{
    textures.resize(textures.size() + 1);
    w.resize(w.size() + 1);
    h.resize(h.size() + 1);

    FILE* f;
    f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header


    w[w.size() - 1] = *(int*)&info[18]; // extract image height and width from header
    h[h.size() - 1] = *(int*)&info[22];

    int size = 3 * w[w.size() - 1] * h[h.size() - 1];
    textures[textures.size() - 1].resize(size); // allocate 3 bytes per pixel
    fread(&textures[textures.size() - 1][0], sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);

    for (int i = 0; i < size; i += 3)
        std::swap(textures[textures.size() - 1][i], textures[textures.size() - 1][i + 2]);
}


Vector Mesh::getNormal(uint tri_id)
{
    return Vector::crossProduct(vertices[indices[tri_id].vtxj] - vertices[indices[tri_id].vtxi],
                                vertices[indices[tri_id].vtxk] - vertices[indices[tri_id].vtxi]).normalizeConst();
}

void Mesh::setNormals()
{
    for (uint i = 0; i < indices.size(); i++)
        tri_normals.push_back(getNormal(i));
}

BBox Mesh::buildBBox(uint tri_0, uint tri_1)
{
    BBox bbox(vertices[indices[tri_0].vtxi], vertices[indices[tri_0].vtxi]);
    for (uint i = tri_0; i < tri_1; i++) // for each triangle
        for (uint j = 0; j < 3; j++) // for each direction
        {
            // for each vertex in triangle
            bbox.P_min[j] = min(bbox.P_min[j], vertices[indices[i].vtxi][j]);
            bbox.P_min[j] = min(bbox.P_min[j], vertices[indices[i].vtxj][j]);
            bbox.P_min[j] = min(bbox.P_min[j], vertices[indices[i].vtxk][j]);

            bbox.P_max[j] = max(bbox.P_max[j], vertices[indices[i].vtxi][j]);
            bbox.P_max[j] = max(bbox.P_max[j], vertices[indices[i].vtxj][j]);
            bbox.P_max[j] = max(bbox.P_max[j], vertices[indices[i].vtxk][j]);
        }

    return bbox;
}


void Mesh::buildBVH(BVH* node, uint i0, uint i1)
{
    node->bbox = buildBBox(i0, i1);
    node->i0 = i0;
    node->i1 = i1;
    node->l_child = nullptr;
    node->r_child = nullptr;

    Vector diag = node->bbox.P_max - node->bbox.P_min;
    uint split_dim;

    // find in which dimension we will split the current bbox
    for (split_dim = 0; split_dim < 3; split_dim++)
        if (diag[split_dim] > diag[(split_dim+1)%3] && diag[split_dim] > diag[(split_dim+2)%3])
            break;

    double split_diag =  node->bbox.P_min[split_dim] + diag[split_dim] * 0.5;

    // pivot
    uint pivot = i0 - 1;
    for (uint i = i0; i < i1; i++)
    {
        double center_split_dim = (vertices[indices[i].vtxi][split_dim] + vertices[indices[i].vtxj][split_dim] + vertices[indices[i].vtxk][split_dim]) / 3.;

        if (center_split_dim < split_diag)
        {
            pivot++;
            swap(indices[i], indices[pivot]);
            swap(tri_normals[i], tri_normals[pivot]);
        }
    }

    if ( pivot <= i0 || pivot >= i1 - 1 || i1 == i0 + 1)
        return;


    // construct children bvh
    node->l_child = new BVH();
    buildBVH(node->l_child, i0, pivot+1);

    node->r_child = new BVH();
    buildBVH(node->r_child, pivot+1, i1);
}




bool Mesh::triIntersection(uint tri_id, const Ray& r, Vector& P, Vector& N, float& t, double& alpha, double& beta, double& gamma) const
{
    N = tri_normals[tri_id];

    Vector A = vertices[indices[tri_id].vtxi];
    Vector B = vertices[indices[tri_id].vtxj];
    Vector C = vertices[indices[tri_id].vtxk];

    // plan / ray intersection
    t = dot(A - r.getOrigin(), N) / dot(r.getDirection(), N);
    P = r.getOrigin() + r.getDirection() * t;

    if (t < 0)
        return false;

    // P in triangle ?
    double APAB = dot(P-A, B-A);
    double APAC = dot(P-A, C-A);
    double ACAB = dot(C-A, B-A);
    double ACAC = dot(C-A, C-A);
    double ABAB = dot(B-A, B-A);

    double det_M = ABAB * ACAC - ACAB * ACAB;
    beta = (APAB * ACAC - APAC * ACAB) / det_M;
    if (beta < 0) return false;
    if (beta > 1) return false;


    gamma = (ABAB * APAC - ACAB * APAB) / det_M;
    if (gamma < 0) return false;
    if (gamma > 1) return false;

    alpha = 1 - beta - gamma;
    if (alpha < 0) return false;
    if (alpha > 1) return false;


    // smooth normal
    N = alpha * normals[indices[tri_id].ni] + beta * normals[indices[tri_id].nj] + gamma * normals[indices[tri_id].nk];
    N.normalize();

    return true;
}


bool Mesh::intersection(const Ray& r, Vector& P, Vector& N, float& t, Vector& color) const
{
    int best_tri = -1;
    float best_t = 1E9;

    double alpha, beta, gamma;

    if (!bvh.bbox.intersection(r))
        return false;

    list<const BVH*> l;
    l.push_front(&bvh);

    while(!l.empty())
    {
        const BVH* current = l.front();
        l.pop_front();

        if (current->l_child != nullptr && current->l_child->bbox.intersection(r))
            l.push_back(current->l_child);

        if (current->r_child != nullptr && current->r_child->bbox.intersection(r))
            l.push_back(current->r_child);

        if (current->l_child == nullptr) // feuille
            for (uint i = current->i0; i < current->i1; i++)
                if (triIntersection(i, r, P, N, t, alpha, beta, gamma))
                    if (t < best_t)
                    {
                        best_t = t;
                        best_tri = i;
                    }
    }

    if (best_tri == -1)
        return false;

    triIntersection(best_tri, r, P, N, t, alpha, beta, gamma);

    // procedural test
    //color = Vector( 0.5 * (1 + cos(P.x)), 0.5 * (1 + cos(P.y)), 0.5 * (1 + cos(P.z)));

    // texture
    uint texture_id = 0;
    Vector uv = alpha * uvs[indices[best_tri].uvi] + beta * uvs[indices[best_tri].uvj] + gamma * uvs[indices[best_tri].uvk];

    uv.x *= (w[texture_id] - 1);
    uv.y *= (h[texture_id] - 1);

    for (uint i = 0; i < 3; i++)
        color[i] = (textures[texture_id][( (int) uv.y * w[texture_id] + (int) uv.x) * 3 + i]) / 255.;

    return true;
}

