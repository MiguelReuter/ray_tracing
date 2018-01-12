#ifndef LIGHT_H
#define LIGHT_H

#include "Vector.h"

class Light
{
    public:
        Light();
        Light(Vector pos, float intensity, Vector color=Vector(255.f, 255.f, 255.f));
        Vector position;
        Vector color;
        float intensity;
        virtual ~Light();

    protected:

    private:
};

#endif // LIGHT_H
