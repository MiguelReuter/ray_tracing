#include "Vector.h"
#include "math.h"


Vector::Vector()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Vector::Vector(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

/*void Vector::operator+=(const Vector v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vector::operator-=(const Vector v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}
*/


const float Vector::length2()
{
    return x * x + y * y + z * z;
}

const float Vector::length()
{
    return sqrt(length2());
}

void Vector::normalize()
{
    float n = length();

    x /= n;
    y /= n;
    z /= n;
}

ostream& operator<<(ostream& os, const Vector& v)
{
    os << '(' << v.x << ', ' << v.y << ', ' << v.z << ')';
    return os;
}

Vector::~Vector()
{
    //dtor
}


