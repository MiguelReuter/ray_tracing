#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

using namespace std;

class Vector
{
    public:
        Vector();
        Vector(float x, float y, float z);
        virtual ~Vector();

        float x, y, z;

        // operators
        Vector inline operator+(const Vector v){ return Vector(x + v.x,
                                                        y + v.y,
                                                        z + v.z);};
        Vector inline operator-(const Vector v){ return Vector(x - v.x,
                                                        y - v.y,
                                                        z - v.z);};


        const float length2();
        const float length();
        void normalize();



        static float dot(Vector u, Vector v){return u.x * v.x + u.y * v.y + u.z * v.z;};

        friend ostream& operator<<(ostream& os, const Vector& v);

        /*
        void operator+=(const Vector v) {}
        void operator-=(const Vector v) {}
*/
        Vector operator*(float a) {return Vector(a*x, a*y, a*z);};

    protected:

    private:
};

#endif // VECTOR_H
