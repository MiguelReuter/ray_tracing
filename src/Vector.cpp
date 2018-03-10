#include "Vector.h"



Vector::Vector()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Vector::Vector(double _x, double _y, double _z)
{
    x = _x;
    y = _y;
    z = _z;
}


double Vector::length2() const
{
    return x * x + y * y + z * z;
}

double Vector::length() const
{
    return sqrt(length2());
}

void Vector::normalize()
{
    double n = length();

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

double Vector::getItem(int i) const
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

double& Vector::operator[](const int i)
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



