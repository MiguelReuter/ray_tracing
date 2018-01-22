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
        Sphere(Vector C, float r, Vector color, bool mirror = false, Vector specular_color = Vector(1.0, 1.0, 1.0), bool transparent=false, float coeff_n=1.0);

        Vector color, specular_color;
        virtual ~Sphere();
        bool intersection(const Ray& r, Vector& P, Vector& N, float& t);

        void setMirror(){is_mirror = true;};
        void setMirror(bool _b){is_mirror = _b;};

        void setTransparent(){is_transparent = true;};
        void setTransparent(bool _b){is_transparent = _b;};

        bool isMirror() const {return is_mirror;};
        bool isTransparent() const {return is_transparent;};
        float coeff_n = 1.0;
    protected:

    private:
        Vector center;
        float radius;
        bool is_mirror = false;
        bool is_transparent = false;
};

#endif // SPHERE_H
