#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include"Rtweekend.h"

#include"Hittable.h"
#include"Material.h"

class Camera
{
public:
	double aspectRatio = 1.0;
	int imageWidth = 100;
	int samplesPerPixel = 10;
	int maxDepth = 10;

	double vfov = 90;
	Point3 lookfrom = Point3(0, 0, 0);
	Point3 lookat = Point3(0, 0, -1);
	Vec3 vup = Vec3(0, 1, 0);

	double defocus_angle = 0;
	double focus_dist = 10;

	void Render(const Hittable& world)
	{
		Initialize();

		std::cout << "P3\n" << imageWidth << ' ' << mImageHeight << "\n255\n";

		for (int scanlineIndex = 0; scanlineIndex < mImageHeight; ++scanlineIndex)
		{
			std::clog
				<< "\rScanlines remaning: "
				<< (mImageHeight - scanlineIndex)
				<< ' '
				<< std::flush;

			for (int pixelIndex = 0; pixelIndex < imageWidth; ++pixelIndex)
			{
				Color pixelColor(0.0, 0.0, 0.0);

				for (int sampleIndex = 0; sampleIndex < samplesPerPixel; ++sampleIndex)
				{
					Ray ray = GetRay(pixelIndex, scanlineIndex);
					pixelColor += RayColor(ray, maxDepth, world);
				}

				WriteColor(std::cout, mPixelSamplesScale * pixelColor);

			}
		}

		std::clog << "\rDone.                       \n";
	}

private:
	void Initialize()
	{
		mImageHeight = static_cast<int>(imageWidth / aspectRatio);
		mImageHeight = (mImageHeight < 1) ? 1 : mImageHeight;
		mPixelSamplesScale = 1.0 / static_cast<double>(samplesPerPixel);

		mCenter = lookfrom;

		//viewport와 카메라의 거리, 픽셀비율에 따른 뷰포트의 크기설정
		auto focalLength = (lookfrom-lookat).Length();
		auto theta = DegreesToRadians(vfov);
		auto h = std::tan(theta / 2);
		auto viewportHeight = 2 * h * focalLength;
		auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / mImageHeight);


		//카메라의 좌표 프레임에 대한 u,v,w 단위 기저 벡터 연산
		w = UnitVector(lookfrom - lookat);
		u = UnitVector(Cross(vup, w));
		v = Cross(w, u);

		//viewport의 수직 수평 벡터
		//수직 벡터는 우리의 공간좌표는 y축이 위로갈수록 증가하지만, 데이터상으로는 아래로 갈 수록 증가하기에
		//-하여 출력시에 원하는 모양으로 이미지가 그려지도록 함
		auto viewportU = viewportWidth * u;
		auto viewportV = viewportHeight * -v;

		//픽셀 하나의 가로 세로 길이를 구함
		mPixelDeltaU = viewportU / imageWidth;
		mPixelDeltaV = viewportV / mImageHeight;


		//카메라의 중심과 viweport의 중심이 같으므로 
		//카메라와 viewport의 거리, viweport의 가로 세로 길이의 반을 뻄으로써 viewport 좌상단 좌표를 구함
		auto viewportUpperLeft =
			mCenter
			- (focalLength * w)
			- viewportU / 2.0
			- viewportV / 2.0;

		mPixel00Location = viewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);

		const double defocusRadius =
			focus_dist * std::tan(DegreesToRadians(defocus_angle * 0.5));

		mDefocusDiskU = u * defocusRadius;
		mDefocusDiskV = v * defocusRadius;
	}

	Ray GetRay(int pixelIndex, int scanlineIndex) const
	{
		auto offset = SampleSquare();

		auto pixelSample =
			mPixel00Location
			+ ((pixelIndex + offset.X()) * mPixelDeltaU)
			+ ((scanlineIndex + offset.Y()) * mPixelDeltaV);

		auto rayOrigin = (defocus_angle<=0.0)?mCenter : DefocusDiskSample();
		auto rayDirection = pixelSample - rayOrigin;

		return Ray(rayOrigin, rayDirection);
	}

	Vec3 SampleSquare() const
	{
		return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0.0);
	}

	Point3 DefocusDiskSample()const
	{
		const Vec3 point = RandomInUnitVector();
		return mCenter + (point.X() * mDefocusDiskU) + (point.Y() * mDefocusDiskV);
	}


	Color RayColor(const Ray& ray,int depth, const Hittable& world) const
	{
		if (depth <= 0)
		{
			return Color(0.0, 0.0, 0.0);
		}
		HitRecord hitRecord;

		if (world.Hit(ray, Interval(0.001, Infinity), hitRecord))
		{
			Ray scattered;
			Color attenuation;

			if (hitRecord.material->Scatter(ray, hitRecord, attenuation, scattered))
			{
				return attenuation * RayColor(scattered, depth - 1, world);
			}
			
			return Color(0.0, 0.0, 0.0);
		}

		Vec3 unitDirection = UnitVector(ray.Direction());
		auto a = 0.5 * (unitDirection.Y() + 1.0);

		return (1.0 - a) * Color(1.0, 1.0, 1.0)
			+ a * Color(0.5, 0.7, 1.0);

	}


private:

	double mPixelSamplesScale = 1.0;
	int mImageHeight = 0;
	Point3 mCenter;
	Point3 mPixel00Location;
	Vec3 mPixelDeltaU;
	Vec3 mPixelDeltaV;
	Vec3 u, v, w;

	Vec3 mDefocusDiskU;
	Vec3 mDefocusDiskV;
};

#endif