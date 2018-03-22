#include "Vector.h"
#include <random>
// random engine
static std::default_random_engine engine;
static std::uniform_real_distribution <double> u(0,1);

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


double dot(const Vector &u, const Vector &v){
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vector random_cos(const Vector &N)
{
    double r1 = u(engine);
    double r2 = u(engine);

    // in local ref
    Vector rand_ray_dir_local = Vector(cos(2*M_PI*r1)*sqrt(1-r2), sin(2*M_PI*r1)*sqrt(1-r2), sqrt(r2));
    Vector rand_vect(u(engine) - 0.5, u(engine) - 0.5, u(engine) - 0.5);

    Vector tang_1 = (Vector::crossProduct(N, rand_vect));
    tang_1.normalize();
    Vector tang_2 = Vector::crossProduct(tang_1, N);

    // in global ref
    return N * rand_ray_dir_local.z + tang_1 * rand_ray_dir_local.x + tang_2 * rand_ray_dir_local.y;
}





