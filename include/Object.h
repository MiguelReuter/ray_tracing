#ifndef OBJECT_H
#define OBJECT_H

#include "Vector.h"
#include "Ray.h"

#include <iostream>

using namespace std;

class Object
{
    public:
        Object();
        virtual ~Object();

        virtual bool intersection(const Ray& r, Vector& P, Vector& N, float& t, Vector& color) const = 0;

        Vector color, specular_color;

        void setMirror(){is_mirror = true;};
        void setMirror(bool _b){is_mirror = _b;};

        void setTransparent(){is_transparent = true;};
        void setTransparent(bool _b){is_transparent = _b;};

        bool isMirror() const {return is_mirror;};
        bool isTransparent() const {return is_transparent;};


        float coeff_n = 1.0;

    protected:
        bool is_mirror = false;
        bool is_transparent = false;

    private:
};

#endif // OBJECT_H
