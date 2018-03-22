#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Mesh.h"
#include "Object.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include <math.h>
#include "Vector.h"


class Scene
{
    public:
        Scene();
        vector<const Object*> objects;
        vector<const Light*> lights;

        virtual ~Scene();
        void addSphere(Sphere const &sph){objects.push_back(&sph);};
        void addTriangle(Triangle const &tri){objects.push_back(&tri);};
        void addMesh(Mesh const &mesh){objects.push_back(&mesh);};

        void addLight(Light const &light){lights.push_back(&light);};

        bool intersection(const Ray& r, Vector& P, Vector& N, int& object_ind, float& t, Vector& color) const;
        bool computeShadow(Vector intersect_pt, Vector light_pos);


        Vector getColor(const Ray& r, int rebound_nb);

    protected:

    private:
};

#endif // SCENE_H
