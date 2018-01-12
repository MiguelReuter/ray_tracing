#include "Scene.h"

Scene::Scene()
{
    //ctor
}



bool Scene::intersect(Ray& r, Vector& P, Vector& N, int& sphere_ind)
{
    float best_d = 1E9;
    int best_ind = -1;

    for (int i=0; i<spheres.size(); i++)
    {
        Sphere sphere = spheres[i];
        Vector P_i, N_i;
        if (sphere.intersect(r, P_i, N_i))
        {
            float d = (P_i - r.origin).length2();
            if (d < best_d)
            {
                best_d = d;
                best_ind = i;
            }
        }
    }

    if (best_ind != -1)
    {
        sphere_ind = best_ind;
        return spheres[best_ind].intersect(r, P, N);
    }

    else
        return false;
}

Scene::~Scene()
{
    //dtor
}
