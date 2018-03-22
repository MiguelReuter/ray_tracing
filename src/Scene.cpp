#include "Scene.h"

using namespace std;

Scene::Scene()
{
    //ctor
}



bool Scene::intersection(const Ray& r, Vector& P, Vector& N, int& object_ind, float& t, Vector& color) const
{
    float best_t = 1E9;
    int best_id = -1;

    for (uint i=0; i < objects.size(); i++)
    {
        //const Object* object = objects[i];
        Vector P_i, N_i;
        float t_i;
        if (objects[i]->intersection(r, P_i, N_i, t_i, color))
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
        object_ind = best_id;
        return objects[best_id]->intersection(r, P, N, t, color);
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

    for (uint i = 0; i < objects.size(); i++)
    {
        const Object* object = objects[i];
        Vector P_i, N_i;
        float t_i;

        Vector color;
        if (object->intersection(r, P_i, N_i, t_i, color))
            if (t_i * t_i <= d_max)
                return true;
    }
    return false;
}


Vector Scene::getColor(const Ray& r, int rebound_nb)
{
    Vector color(0.0, 0.0, 0.0);

    if (rebound_nb == 0)
        return color;


    for (uint i = 0; i < lights.size(); i++)
    {
        // light
        const Light* light = lights[i];

        Vector P, N;
        float t_light;
        int object_id;

        Vector obj_color;

        // if ray hit an object
        if (intersection(r, P, N, object_id, t_light, obj_color))
        {
            const Object* object = objects[object_id];

            // Specular
            if (object->isMirror())
            {
                Vector mirror_ray_dir = r.getDirection() - N * 2 * dot(N, r.getDirection());
                Ray r_mirror(P + N * 0.01, mirror_ray_dir);

                color = getColor(r_mirror, rebound_nb - 1);

                // spec color
                color = object->specular_color * color;
            }
            else if (object->isTransparent())
            {
                float n1 = 1.0;
                float n2 = object->coeff_n;
                Vector normal_refraction = N;

                if (dot(N, r.getDirection()) > 0) // ray comes from the sphere
                {
                    n1 = object->coeff_n;
                    n2 = 1.0;
                    normal_refraction = N * -1;
                }

                float radical = 1 - pow(n1 / n2, 2) * (1 - pow(dot(r.getDirection(), normal_refraction), 2));
                if (radical >= 0)
                {
                    Vector transmitted_ray_t = (r.getDirection() - (normal_refraction * dot(r.getDirection(), normal_refraction))) * (n1 / n2);
                    Vector transmitted_ray_n = normal_refraction * -sqrt(radical);
                    Ray r_transp(P - normal_refraction * 0.01, transmitted_ray_n + transmitted_ray_t);

                    color = getColor(r_transp, rebound_nb - 1);
                }

            }
            else
            {
                // direct lighting
                Vector lum_P_axis = (P - light->position).normalizeConst();
                Vector random_dir = random_cos(lum_P_axis);
                Vector random_point = light->radius * random_dir + light->position;
                Vector w_i = (random_point - P).normalizeConst();
                Vector Np = random_dir;

                double d_light_2 = (random_point - P).length2();

                if (!computeShadow(P + N * 0.01, random_point))
                    color = (light->intensity / (4 * M_PI * d_light_2) * max(0., dot(N, w_i) * dot(Np, -1*w_i) / dot(lum_P_axis, random_dir)) ) * obj_color * light->color;

                // indirect lighting
                Vector rand_ray_dir = random_cos(N);

                Ray rand_ray(P + N * 0.01, rand_ray_dir);
                color += getColor(rand_ray, rebound_nb - 1) * obj_color;
            }
        }
    }

    return color;
}




Scene::~Scene()
{
    //dtor
}
