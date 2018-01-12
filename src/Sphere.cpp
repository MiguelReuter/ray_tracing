#include "Sphere.h"
#include <iostream>

using namespace std;
Sphere::Sphere()
{
    center = Vector(0.0f, 0.0f, 0.0f);
    radius = 1.0;
    color = Vector(1.0, 1.0, 1.0);
}

Sphere::Sphere(Vector C, float r, Vector col)
{
    center = C;
    radius = r;
    color = col;
}


bool Sphere::intersect(Ray& r, Vector& P, Vector& N)
{
    float a = (r.direction).length2();
    float b = 2 * Vector::dot(r.direction, r.origin - center);
    float c = (center - r.origin).length2() - radius * radius;

    float delta = b*b - 4 * a * c;

    if (delta < 0)
        return false;
    else
    {
        float t_1 = (-b + sqrt(delta)) / 2 * a;
        float t_2 = (-b - sqrt(delta)) / 2 * a;
        if (t_1 >= 0) //we check only for t_1 ( > t_2)
        {
            Vector P_1 = r.origin + r.direction * t_1;
            Vector P_2 = r.origin + r.direction * t_2;

            float d_1 = (P_1 - r.origin).length2();
            float d_2 = (P_2 - r.origin).length2();

            if (t_1 >= 0 && d_1 < d_2)
                P = P_1;
            else
                P = P_2;

            N = (P - center);
            N.normalize();
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
