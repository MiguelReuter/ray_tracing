// Raytracer.cpp : Defines the entry point for the console application.
#define _CRT_SECURE_NO_WARNINGS // for AVisual Studio 2017 (maybe 2015 as well)

#define NUM_RAYS 5
#define NUM_BOUNDS 3


#include "Light.h"
#include "Ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
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
	double shutter_size = 0.5;
	double d_camera = 55;

	// scene
	Scene scene;
	// light
	scene.addLight(Light(Vector(15, 70, -30), 3000000));
	//scene.addLight(Light(Vector(10, 20, 40), 1000000));

	// objects
	// mirror sphere
/*
    Sphere s_mirror = Sphere(Vector(-25.0, 0.0, -55.0), 10.0f, Vector(1.0, 0.0, 0.0), true);
    scene.addSphere(s_mirror);

    // transparent sphere
    Sphere s_transp = Sphere(Vector(25.0, 0.0, -55.0), 10.0f, Vector(1.0, 0.0, 0.0));
    s_transp.setTransparent();
    s_transp.coeff_n = 1.3;
    scene.addSphere(s_transp);*/

    // diffuse sphere
    Sphere sph_1(Vector(-15.0, 0.0, -55.0), 5.0f, Vector(1.0, 1.0, 1.0));
    Sphere sph_2(Vector(15.0, 0.0, -55.0), 5.0f, Vector(.7, 1.0, 1.0));
    Sphere sph_3(Vector(0.0, 20.0, -55.0), 5.0f, Vector(1.0, 1.0, .5));

    scene.addSphere(sph_1);
    scene.addSphere(sph_2);
    scene.addSphere(sph_3);

    Triangle tri_1(Vector{-15.0, 0.0, -55.0}, Vector{15.0, 0.0, -55.0}, Vector{0.0, 20.0, -55.0});
    tri_1.color = Vector{1.0, 1.0, 0.2};
    scene.addTriangle(tri_1);
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
                // ----------   ANTI ALIASING   ---------- //
                // Box Muller method
                double r1 = uniform_rand(engine_rand);
                double r2 = uniform_rand(engine_rand);
                double _r = sqrt(-2*log(r1));

                double dx = _r * cos(2*M_PI*r2);
                double dy = _r * sin(2*M_PI*r2);

                // ray (from camera to each pixel in pixel grid)
                Vector direction = Vector(j - W/2.0f + 0.5 + dx, i - H/2.0f + 0.5 + dy, -W / (2 * tan_fov)).normalizeConst();

                // profondeur de champ
                r1 = uniform_rand(engine_rand);
                r2 = uniform_rand(engine_rand);
                _r = sqrt(-2*log(r1));
                dx = shutter_size * _r * cos(2*M_PI*r2);
                dy = shutter_size * _r * sin(2*M_PI*r2);

                direction = (camera_pos - Vector(dx, dy, 0) + direction * d_camera).normalizeConst();
                Ray r(camera_pos + Vector(dx, dy, 0), direction);

                color += scene.getColor(r, NUM_BOUNDS);

            }

            color /= NUM_RAYS;


            img[((H-i-1)*W + j)*3 + 0] = min(255., pow(color[0], 0.4545));
            img[((H-i-1)*W + j)*3 + 1] = min(255., pow(color[1], 0.4545));
            img[((H-i-1)*W + j)*3 + 2] = min(255., pow(color[2], 0.4545));
        }
    }

	save_image("seance_5_.bmp", &img[0], W, H);

    return 0;
}
