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
        Vector(double x, double y, double z);
        virtual ~Vector();

        double x, y, z;
        double getItem(int i) const;

        double length2() const;
        double length() const;
        void normalize();
        Vector normalizeConst() const;

        static Vector crossProduct(const Vector& u, const Vector& v){return Vector(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);};

        friend ostream& operator<<(ostream& os, const Vector& v);
        friend Vector operator*(double a, const Vector& v) {return v * a;};

// operators
        Vector inline operator+(const Vector v) const { return Vector(x + v.x, y + v.y, z + v.z);};
        Vector inline operator-(const Vector v) const { return Vector(x - v.x, y - v.y, z - v.z);};
        Vector operator*(double a) const {return Vector(a*x, a*y, a*z);};
        Vector operator/(double a) const {return Vector(x/a, y/a, z/a);};
        Vector operator*(Vector b) const {return Vector(x*b.x, y*b.y, z*b.z);};

        Vector& operator+=(Vector b) {x += b.x; y += b.y; z += b.z; return *this;};
        Vector& operator-=(Vector b) {x -= b.x; y -= b.y; z -= b.z; return *this;};
        Vector& operator*=(Vector b) {x *= b.x; y *= b.y; z *= b.z; return *this;};
        Vector& operator/=(double b) {x /= b; y /= b; z /= b; return *this;};

        double& operator[](const int i);

    protected:

    private:
};

double dot(const Vector &u, const Vector &v);
Vector random_cos(const Vector &N);

#endif // VECTOR_H
