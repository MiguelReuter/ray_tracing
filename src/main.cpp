// Raytracer.cpp : Defines the entry point for the console application.
#define _CRT_SECURE_NO_WARNINGS // for AVisual Studio 2017 (maybe 2015 as well)


#include "Light.h"
#include "Ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "Vector.h"


#include "math.h"
#include <iostream>
#include <vector>

#include <iostream>


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

	// objects



	//light
	Vector light_pos = Vector(-10, 20, 40);
	Light light = Light(light_pos, 1000000);

	// camera
	Vector camera_pos = Vector(.0f, .0f, 55.0f);


	Scene scene = Scene();
    scene.add_sphere(Sphere(Vector(5.0, 0.0, 0.0), 10.0f, Vector(0.0, 1.0, 1.0)));
    scene.add_sphere(Sphere(Vector(-5.0, 0.0, 0.0), 10.0f, Vector(1.0, 0.0, 0.0)));

	float fov = 60; // in degrees

	for (int j=0; j<W; j++)
        for (int i=0; i<H; i++)
        {
            Vector u = Vector(j - W/2.0f + 0.5,
                              i - H/2.0f + 0.5,
                              -W / (2 * tan(fov * M_PI / 360.0f)));
            u.normalize();

            Ray r(camera_pos, u);
            Vector P, N;
            int sphere_ind;

            if (scene.intersect(r, P, N, sphere_ind))
            {
                Sphere sphere = scene.spheres[sphere_ind];

                Vector l = light.position - P;
                l.normalize();
                float inte = light.intensity * max(Vector::dot(N, l), .0f) / (light.position - P).length2() * 1/ M_PI;

                img[((H-i-1)*W + j)*3] = min(255.f, inte * sphere.color.x * light.color.x / 255.f);
                img[((H-i-1)*W + j)*3 + 1] = min(255.f, inte * sphere.color.y * light.color.y / 255.f);
                img[((H-i-1)*W + j)*3 + 2] = min(255.f, inte * sphere.color.z * light.color.z / 255.f) ;
            }
        }







	//img[(10 * W + 50)*3] = 255;  // pixel at (x, y) = (50, 10) is red

	save_image("test.bmp", &img[0], W, H);

    return 0;
}
