#ifndef LIGHT_H
#define LIGHT_H

#include "Vector.h"

class Light
{
    public:
        Light();
        Light(Vector pos, float intensity, Vector color=Vector(1.f, 1.f, 1.f), double radius=5);
        Vector position;
        Vector color;
        float intensity;
        double radius;
        virtual ~Light();

    protected:

    private:
};

#endif // LIGHT_H
