#ifndef RAY_H
#define RAY_H

#include "Vector.h"


class Ray
{
    public:
        Ray();
        Ray(Vector O, Vector dir);
        virtual ~Ray();

        Vector origin;
        Vector direction;

    protected:

    private:

};

#endif // RAY_H
