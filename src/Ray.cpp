#include "Ray.h"

Ray::Ray()
{
    direction = Vector(1.0, 0.0, 0.0);
    origin = Vector();
    //ctor
}

Ray::Ray(Vector O, Vector dir)
{
    origin = O;
    direction = dir;
}

Ray::~Ray()
{
    //dtor
}
