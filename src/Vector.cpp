#include "Vector.h"



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


float Vector::length2() const
{
    return x * x + y * y + z * z;
}

float Vector::length() const
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

Vector Vector::normalizeConst() const
{
    Vector u = *this;
    u.normalize();

    return u;
}


ostream& operator<<(ostream& os, const Vector& v)
{
    os << "(" <<  to_string(v.x) << ", " << to_string(v.y) << ", " << to_string(v.z) << ")";
    return os;
}

float Vector::operator[](int i)
{
    if (i == 0)
        return x;
    else if (i == 1)
        return y;
    else if (i == 2)
        return z;
    else
        throw std::out_of_range("");
}

Vector::~Vector()
{
    //dtor
}



