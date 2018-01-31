// Raytracer.cpp : Defines the entry point for the console application.
#define _CRT_SECURE_NO_WARNINGS // for AVisual Studio 2017 (maybe 2015 as well)
# define NUM_RAYS 100

#include "Light.h"
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
	scene.addLight(Light(Vector(15, 70, -30), 1000000));
	//scene.addLight(Light(Vector(10, 20, 40), 1000000));

	// objects
	// mirror sphere

    Sphere s_mirror = Sphere(Vector(-25.0, 0.0, -55.0), 10.0f, Vector(1.0, 0.0, 0.0), true);
    scene.addSphere(s_mirror);

    // transparent sphere
    Sphere s_transp = Sphere(Vector(25.0, 0.0, -55.0), 10.0f, Vector(1.0, 0.0, 0.0));
    s_transp.setTransparent();
    s_transp.coeff_n = 1.3;
    scene.addSphere(s_transp);

    // diffuse sphere
    scene.addSphere(Sphere(Vector(0.0, 0.0, -55.0), 10.0f, Vector(1.0, 1.0, 1.0)));

    // walls
    scene.addSphere(Sphere(Vector(0.0, -2000.0-20, 0.0), 2000.0f, Vector(1.0, 1.0, 1.0))); // ground
    scene.addSphere(Sphere(Vector(0.0, 2000.0+100, 0.0), 2000.0f, Vector(1.0, 1.0, 1.0))); //ceiling
    scene.addSphere(Sphere(Vector(-2000.0-50, 0.0, 0.0), 2000.0f, Vector(0.0, 1.0, 0.0))); // left wall
    scene.addSphere(Sphere(Vector(2000.0+50, 0.0, 0.0), 2000.0f, Vector(0.0, 0.0, 1.0))); // right wall
    scene.addSphere(Sphere(Vector(0.0, 0.0, -2000.0-100), 2000.0f, Vector(0.0, 1.0, 1.0))); // back wall

#pragma omp parallel for
	for (int j=0; j<W; j++)
	{
        for (int i=0; i<H; i++)
        {
            // ray (from camera to each pixel in pixel grid)
            Vector direction = Vector(j - W/2.0f + 0.5, i - H/2.0f + 0.5, -W / (2 * tan_fov)).normalizeConst();
            Ray r(camera_pos, direction);

            Vector color(0.0, 0.0, 0.0);
            for (int k = 0; k < NUM_RAYS; k++)
                color += scene.getColor(r, 5);

            color /= NUM_RAYS;


            img[((H-i-1)*W + j)*3 + 0] = min(255., pow(color[0], 0.4545));
            img[((H-i-1)*W + j)*3 + 1] = min(255., pow(color[1], 0.4545));
            img[((H-i-1)*W + j)*3 + 2] = min(255., pow(color[2], 0.4545));
        }
    }

	save_image("seance_2_.bmp", &img[0], W, H);

    return 0;
}
