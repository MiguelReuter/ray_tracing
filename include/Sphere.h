#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include "math.h"

#include "Object.h"
#include "Vector.h"
#include "Ray.h"

class Sphere : public Object
{
    public:
        Sphere();
        Sphere(Vector C, float r, Vector color, bool mirror = false, Vector specular_color = Vector(1.0, 1.0, 1.0), bool transparent=false, float coeff_n=1.0);

        virtual ~Sphere();
        virtual bool intersection(const Ray& r, Vector& P, Vector& N, float& t, Vector& color) const;

    protected:

    private:
        Vector center;
        float radius = 1.0;

};

#endif // SPHERE_H
