#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <string>

#include "math.h"

using namespace std;

class Vector
{
    public:
        Vector();
        Vector(float x, float y, float z);
        virtual ~Vector();

        float x, y, z;

        float length2() const;
        float length() const;
        void normalize();
        Vector normalizeConst() const;

        static float dot(Vector u, Vector v){return u.x * v.x + u.y * v.y + u.z * v.z;};

        friend ostream& operator<<(ostream& os, const Vector& v);

// operators
        Vector inline operator+(const Vector v){ return Vector(x + v.x, y + v.y, z + v.z);};
        Vector inline operator-(const Vector v){ return Vector(x - v.x, y - v.y, z - v.z);};
        Vector operator*(float a) {return Vector(a*x, a*y, a*z);};
        float operator[](int i);

    protected:

    private:
};

#endif // VECTOR_H
