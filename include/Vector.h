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
        static Vector crossProduct(const Vector& u, const Vector& v){return Vector(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);};

        friend ostream& operator<<(ostream& os, const Vector& v);

// operators
        Vector inline operator+(const Vector v){ return Vector(x + v.x, y + v.y, z + v.z);};
        Vector inline operator-(const Vector v){ return Vector(x - v.x, y - v.y, z - v.z);};
        Vector operator*(float a) {return Vector(a*x, a*y, a*z);};
        Vector operator/(float a) {return Vector(x/a, y/a, z/a);};
        Vector operator*(Vector b) {return Vector(x*b.x, y*b.y, z*b.z);};

        Vector& operator+=(Vector b) {x += b.x; y += b.y; z += b.z; return *this;};
        Vector& operator-=(Vector b) {x -= b.x; y -= b.y; z -= b.z; return *this;};
        Vector& operator*=(Vector b) {x *= b.x; y *= b.y; z *= b.z; return *this;};
        Vector& operator/=(float b) {x /= b; y /= b; z /= b; return *this;};

        float operator[](int i);

    protected:

    private:
};

#endif // VECTOR_H
