#include "Light.h"

Light::Light()
{
    position = Vector();
    intensity = 100.0;
    color = Vector(1.f, 1.f, 1.f);
    radius = 1.;
}


Light::Light(Vector pos, float _int, Vector col, double r)
{
    position = pos;
    intensity = _int;
    color = col;
    radius = r;
}

Light::~Light()
{
    //dtor
}
