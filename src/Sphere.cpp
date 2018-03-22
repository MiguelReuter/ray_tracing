#include "Sphere.h"
#include <iostream>

using namespace std;
Sphere::Sphere()
{
    center = Vector(0.0f, 0.0f, 0.0f);
    radius = 1.0;
    color = Vector(1.0, 1.0, 1.0);
}

Sphere::Sphere(Vector C, float r, Vector col, bool mirror, Vector spec_col, bool transparent, float n) : center(C), radius(r)
{
    color = col;
    is_mirror = mirror;
    is_transparent = transparent;
    specular_color = spec_col;
    coeff_n = n;
};

bool Sphere::intersection(const Ray& r, Vector& P, Vector& N, float& t, Vector& color) const
{
    color = this->color;

    float a = (r.getDirection()).length2();
    float b = 2 * dot(r.getDirection(), r.getOrigin() - center);
    float c = (center - r.getOrigin()).length2() - radius * radius;

    float delta = b*b - 4 * a * c;

    if (delta < 0)
        return false;
    else
    {
        float t_1 = (-b + sqrt(delta)) / 2 * a;
        if (t_1 >= 0) //we check only for t_1 ( > t_2)
        {
            float t_2 = (-b - sqrt(delta)) / 2 * a;

            if (t_2 >= 0)
                t = t_2;
            else
                t = t_1;

            P = r.getOrigin() + r.getDirection() * t;
            N = (P - center).normalizeConst();
            return true;
        }

        else
            return false;
    }
return false;
}


Sphere::~Sphere()
{
    //dtor
}
