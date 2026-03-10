#include "RTWeekend.h"

#include "Hittable.h"
#include"Hittable_List.h"
#include"Sphere.h"
#include"Camera.h"
#include"Material.h"
#include"Metal.h"


#include <iostream>
#include"Color.h"
#include"Vec3.h"
#include"Ray.h"
#include"Interval.h"

using namespace std;





int main()
{
	

	//world
	HittableList world;


	auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	world.Add(std::make_shared<Sphere>(Point3(0.0, -1000.0, 0.0), 1000.0, groundMaterial));

	for (int gridX = -11; gridX < 11; ++gridX)
	{
		for (int gridZ = -11; gridZ < 11; ++gridZ)
		{
			const double materialSelector = RandomDouble();

			const Point3 center(
				gridX + 0.9 * RandomDouble(),
				0.2,
				gridZ + 0.9 * RandomDouble()
			);

			if ((center - Point3(4.0, 0.2, 0.0)).Length() > 0.9)
			{
				std::shared_ptr<Material> sphereMaterial;

				if (materialSelector < 0.8)
				{
					const Color albedo = Color::Random() * Color::Random();
					sphereMaterial = std::make_shared<Lambertian>(albedo);
				}
				else if (materialSelector < 0.95)
				{
					const Color albedo = Color::Random(0.5, 1.0);
					const double fuzz = RandomDouble(0.0, 0.5);
					sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
				}
				else
				{
					sphereMaterial = std::make_shared<Dielectric>(1.5);
				}

				world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5);
	world.Add(std::make_shared<Sphere>(Point3(0.0, 1.0, 0.0), 1.0, material1));

	auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	world.Add(std::make_shared<Sphere>(Point3(-4.0, 1.0, 0.0), 1.0, material2));

	auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.Add(std::make_shared<Sphere>(Point3(4.0, 1.0, 0.0), 1.0, material3));


	//camera

	Camera camera;
	camera.aspectRatio = 16.0 / 9.0;
	camera.imageWidth = 1200;
	camera.samplesPerPixel = 10;
	camera.maxDepth = 20;

	camera.vfov = 20;
	camera.lookfrom = Point3(13.0, 2.0, 3.0);
	camera.lookat = Point3(0, 0, 0);
	camera.vup = Vec3(0, 1, 0);

	camera.defocus_angle = 0.6;
	camera.focus_dist = 10.0;

	camera.Render(world);
	return 0;
}