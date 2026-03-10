#pragma once

#include"Material.h"

class Metal :public Material
{
public:
	explicit Metal(const Color& albedo,double fuzz)
		:mAlbedo(albedo)
		,mFuzz(fuzz<1?fuzz:1)
	{
	}

	bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation,
		Ray& scattered) const override
	{
		Vec3 reflected = Reflect(rayIn.Direction(), hitRecord.Normal);
		reflected = UnitVector(reflected) + (mFuzz * RandomUnitVector());
		scattered = Ray(hitRecord.P, reflected);
		attenuation = mAlbedo;
		
		return (Dot(scattered.Direction(), hitRecord.Normal) > 0);
	}
private:
	Color mAlbedo;
	double mFuzz;
};