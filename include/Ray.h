#ifndef RAY_H
#define RAY_H

#include "Vector.h"


class Ray
{
    public:
        Ray();
        Ray(Vector origin, Vector direction);
        virtual ~Ray();

        Vector getOrigin() const {return origin;};
        Vector getDirection() const {return direction;};

    protected:

    private:
        Vector origin;
        Vector direction;
};

#endif // RAY_H
