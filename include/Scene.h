#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Sphere.h"
#include "Light.h"

class Scene
{
    public:
        Scene();
        vector<Sphere> spheres;
        vector<Light> lights;

        virtual ~Scene();
        void addSphere(Sphere sph){spheres.push_back(sph);};
        void addLight(Light light){lights.push_back(light);};

        bool intersection(const Ray& r, Vector& P, Vector& N, int& sphere_ind, float& t);
        bool computeShadow(Vector intersect_pt, Vector light_pos);

        Vector getColor(const Ray& r, int rebound_nb);

    protected:

    private:
};

#endif // SCENE_H
