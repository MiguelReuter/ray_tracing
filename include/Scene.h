#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Sphere.h"

class Scene
{
    public:
        Scene();
        vector<Sphere> spheres;
        virtual ~Scene();
        void add_sphere(Sphere sph){spheres.push_back(sph);};


        bool intersect(Ray& r, Vector& P, Vector& N, int& sphere_ind);

    protected:

    private:
};

#endif // SCENE_H
