#include "Light.h"

Light::Light()
{
    position = Vector();
    intensity = 100.0;
    color = Vector(255.f, 255.f, 255.f);
}


Light::Light(Vector pos, float _int, Vector col)
{
    position = pos;
    intensity = _int;
    color = col;
}

Light::~Light()
{
    //dtor
}
