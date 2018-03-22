// Raytracer.cpp : Defines the entry point for the console application.
#define _CRT_SECURE_NO_WARNINGS // for AVisual Studio 2017 (maybe 2015 as well)

#define NUM_RAYS 3
#define NUM_BOUNDS 3

#define ANTIALIASING true
#define DOF true

double shutter_size = 1.0;
double focus_distance = 60;


#include "Light.h"
#include "Mesh.h"
#include "Ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "Vector.h"

#include "math.h"
#include "omp.h"
#include <iostream>
#include <random>
#include <vector>


using namespace std;

// random engine
std::default_random_engine engine_rand;
std::uniform_real_distribution <double> uniform_rand(0,1);



void save_image(const char* filename, const unsigned char* tableau, int w, int h) { // (0,0) is top-left corner

	FILE *f;

	int filesize = 54 + 3 * w*h;

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);
	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	f = fopen(filename, "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	unsigned char *row = new unsigned char[w * 3];
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++) {
			row[j * 3] = tableau[(w*(h - i - 1) * 3) + j * 3+2];
			row[j * 3+1] = tableau[(w*(h - i - 1) * 3) + j * 3+1];
			row[j * 3+2] = tableau[(w*(h - i - 1) * 3) + j * 3];
		}
		fwrite(row, 3, w, f);
		fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
	}
	fclose(f);
	delete[] row;
}

int main()
{
	int W = 1024;
	int H = 1024;
	std::vector<unsigned char> img(W*H * 3, 0);

	// camera
	Vector camera_pos(.0f, .0f, 20.0f);
	float tan_fov = tan(60 * M_PI / 360.0f); // 60 : in degrees

	// scene
	Scene scene;
	// light
	Light light = Light(Vector(15, 70, -30), 255*13000000.);
	light.radius = 10;
	scene.addLight(light);

    // spheres
    Sphere sph_1(Vector(10.0, 0.0, -65.0), 10.0f, Vector(0., .7, 1.0));
    Sphere sph_2(Vector(17.5, 0.0, -40.0), 10.0f, Vector(1., 1.0, 1.0));
    Sphere sph_3(Vector(25.0, 0.0, -15.0), 10.0f, Vector(1.0, .2, .2));

    sph_2.setTransparent();
    sph_2.coeff_n = 1.3;

    scene.addSphere(sph_1);
    scene.addSphere(sph_2);
    scene.addSphere(sph_3);

    Mesh mesh_1 = Mesh("models/Rabbit/Rabbit.obj", 50.0, Vector{-15, -20.5, -55});
    //mesh_1.specular_color = Vector(1., .5, 1.);
    //mesh_1.setMirror();
    scene.addMesh(mesh_1);

    // walls
    Sphere wall_1(Vector(0.0, -2000.0-20, 0.0), 2000.0f, Vector(1.0, 1.0, 1.0));
    Sphere wall_2(Vector(0.0, 2000.0+100, 0.0), 2000.0f, Vector(1.0, 1.0, 1.0));
    Sphere wall_3(Vector(-2000.0-50, 0.0, 0.0), 2000.0f, Vector(0.0, 1.0, 0.0));

    Sphere wall_4(Vector(2000.0+50, 0.0, 0.0), 2000.0f, Vector(0.0, 0.0, 1.0));
    Sphere wall_5(Vector(0.0, 0.0, -2000.0-100), 2000.0f, Vector(0.0, 1.0, 1.0));

    scene.addSphere(wall_1); // ground
    scene.addSphere(wall_2); //ceiling
    scene.addSphere(wall_3); // left wall
    scene.addSphere(wall_4); // right wall
    scene.addSphere(wall_5); // back wall

    #pragma omp parallel for
	for (int j=0; j<W; j++)
    {
        for (int i=0; i<H; i++)
        {
            Vector color(0.0, 0.0, 0.0);
            for (int k = 0; k < NUM_RAYS; k++)
            {
                Vector direction;

                if (ANTIALIASING)
                {
                    // Box Muller method
                    double r1 = uniform_rand(engine_rand);
                    double r2 = uniform_rand(engine_rand);
                    double _r = sqrt(-2*log(r1));

                    double dx = _r * cos(2*M_PI*r2);
                    double dy = _r * sin(2*M_PI*r2);

                    direction = Vector(j - W/2.0f + 0.5 + dx, i - H/2.0f + 0.5 + dy, -W / (2 * tan_fov)).normalizeConst();
                }
                else
                    direction = Vector(j - W/2.0f + 0.5, i - H/2.0f + 0.5 , -W / (2 * tan_fov)).normalizeConst();

                Vector ray_origin;
                if (DOF)
                {
                    // ----------   DEPTH OF FIELD   --------- //
                    double r1 = uniform_rand(engine_rand);
                    double r2 = uniform_rand(engine_rand);
                    double _r = sqrt(-2*log(r1));
                    double dx = shutter_size * _r * cos(2*M_PI*r2);
                    double dy = shutter_size * _r * sin(2*M_PI*r2);

                    Vector destination = camera_pos + focus_distance * direction;

                    ray_origin = camera_pos + Vector(dx, dy, 0);
                    direction = (destination - ray_origin).normalizeConst();
                }
                else
                    ray_origin = camera_pos;

                Ray r(ray_origin, direction);

                color += scene.getColor(r, NUM_BOUNDS);
            }

            color /= NUM_RAYS;


            img[((H-i-1)*W + j)*3 + 0] = min(255., pow(color[0], 0.4545));
            img[((H-i-1)*W + j)*3 + 1] = min(255., pow(color[1], 0.4545));
            img[((H-i-1)*W + j)*3 + 2] = min(255., pow(color[2], 0.4545));
        }
    }

	save_image("output.bmp", &img[0], W, H);

    return 0;
}
