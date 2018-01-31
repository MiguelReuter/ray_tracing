#include "Scene.h"
#include <random>

using namespace std;

// random engine
std::default_random_engine engine;
std::uniform_real_distribution <double> u(0,1);

Scene::Scene()
{
    //ctor
}



bool Scene::intersection(const Ray& r, Vector& P, Vector& N, int& sphere_ind, float& t)
{
    float best_t = 1E9;
    int best_id = -1;

    for (int i=0; i < spheres.size(); i++)
    {
        Sphere sphere = spheres[i];
        Vector P_i, N_i;
        float t_i;

        if (sphere.intersection(r, P_i, N_i, t_i))
        {
            if (t_i < best_t)
            {
                best_t = t_i;
                best_id = i;
            }
        }
    }

    if (best_id != -1)
    {
        sphere_ind = best_id;
        return spheres[best_id].intersection(r, P, N, t);
    }

    else
        return false;
}


bool Scene::computeShadow(Vector intersect_point, Vector light_pos)
{
    Vector LP = light_pos - intersect_point;
    Vector dir = LP.normalizeConst();

    Ray r = Ray(intersect_point, dir);

    float d_max = LP.length2();

    for (int i = 0; i < spheres.size(); i++)
    {
        Sphere sphere = spheres[i];
        Vector P_i, N_i;
        float t_i;

        if (sphere.intersection(r, P_i, N_i, t_i))
        {
            if (t_i * t_i <= d_max)
                return true;
        }
    }
    return false;
}


Vector Scene::getColor(const Ray& r, int rebound_nb)
{
    Vector color(0.0, 0.0, 0.0);

    if (rebound_nb == 0)
        return Vector(0.0, 0.0, 0.0);


    for (int i = 0; i < lights.size(); i++)
    {
        // light
        Light light = lights[i];

        Vector P, N;
        float t_light;
        int sphere_id;

        // if ray hit an object
        if (intersection(r, P, N, sphere_id, t_light))
        {
            Sphere sphere = spheres[sphere_id];

            // Specular
            if (sphere.isMirror())
            {
                Vector mirror_ray_dir = r.getDirection() - N * 2 * Vector::dot(N, r.getDirection());
                Ray r_mirror(P + N * 0.01, mirror_ray_dir);

                color = getColor(r_mirror, rebound_nb - 1);

                // spec color
                color.x = sphere.specular_color[0] * color[0];
                color.y = sphere.specular_color[1] * color[1];
                color.z = sphere.specular_color[2] * color[2];
            }
            else if (sphere.isTransparent())
            {
                float n1 = 1.0;
                float n2 = sphere.coeff_n;
                Vector normal_refraction = N;

                if (Vector::dot(N, r.getDirection()) > 0) // ray comes from the sphere
                {
                    n1 = sphere.coeff_n;
                    n2 = 1.0;
                    normal_refraction = N * -1;
                }

                float radical = 1 - pow(n1 / n2, 2) * (1 - pow(Vector::dot(r.getDirection(), normal_refraction), 2));
                if (radical >= 0)
                {
                    Vector transmitted_ray_t = (r.getDirection() - (normal_refraction * Vector::dot(r.getDirection(), normal_refraction))) * (n1 / n2);
                    Vector transmitted_ray_n = normal_refraction * -sqrt(radical);
                    Ray r_transp(P - normal_refraction * 0.01, transmitted_ray_n + transmitted_ray_t);

                    color = getColor(r_transp, rebound_nb - 1);
                }

            }
            else
            {
                // direct lighting
                if (!computeShadow(P + N * 0.01, light.position))
                {
                    Vector l = (light.position - P).normalizeConst();

                    float intensity_value = light.intensity * max(Vector::dot(N, l), .0f) / (light.position - P).length2() * 1/ M_PI;

                    color.x += intensity_value * sphere.color[0] * light.color[0];
                    color.y += intensity_value * sphere.color[1] * light.color[1];
                    color.z += intensity_value * sphere.color[2] * light.color[2];
                }


                // indirect lighting
                double r1 = u(engine);
                double r2 = u(engine);

                // in local ref
                Vector rand_ray_dir_local = Vector(cos(2*M_PI*r1)*sqrt(1-r2), sin(2*M_PI*r1)*sqrt(1-r2), sqrt(r2));
                Vector rand_vect(u(engine) - 0.5, u(engine) - 0.5, u(engine) - 0.5);

                Vector tang_1 = (Vector::crossProduct(N, rand_vect));
                tang_1.normalize();
                Vector tang_2 = Vector::crossProduct(tang_1, N);


                // in global ref
                Vector rand_ray_dir = N * rand_ray_dir_local.z + tang_1 * rand_ray_dir_local.x + tang_2 * rand_ray_dir_local.y;

                //cout << rand_ray_dir << endl;

                Ray rand_ray(P + N * 0.01, rand_ray_dir);
                color += getColor(rand_ray, rebound_nb - 1) * sphere.color;
            }
        }
    }

    return color;
}




Scene::~Scene()
{
    //dtor
}
