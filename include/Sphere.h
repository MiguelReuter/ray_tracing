#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include "math.h"
#include "Vector.h"
#include "Ray.h"

class Sphere
{
    public:
        Sphere();
        Sphere(Vector C, float r, Vector color);

        Vector color;
        virtual ~Sphere();


        bool intersect(Ray& r, Vector& P, Vector& N);




    protected:

    private:
        Vector center;
        float radius;
};

#endif // SPHERE_H
