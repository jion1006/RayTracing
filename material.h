#pragma once

#include"Hittable.h"

class Material
{
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const
	{
		return false;
	}
};


class Lambertian :public Material
{
public:
	explicit Lambertian(const Color& albedo)
		:mAlbedo(albedo)
	{
	}

	bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation,
		Ray& scattered) const override
	{
		auto scatterDirecion = hitRecord.Normal + RandomUnitVector();

		if (scatterDirecion.NearZero())
		{
			scatterDirecion = hitRecord.Normal;
		}


		scattered = Ray(hitRecord.P, scatterDirecion);
		attenuation = mAlbedo;

		return true;
	}

private:
	Color mAlbedo;
};

class Dielectric :public Material
{
public:
	explicit Dielectric(double refractionIndex)
		:mRefractionIndex(refractionIndex)
	{
	}

	bool Scatter(
		const Ray& rayIn,
		const HitRecord& hitRecord,
		Color& attenuation,
		Ray& scattered
	) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);



		const double refractionRatio =
			hitRecord.bFrontFace ? (1.0 / mRefractionIndex) : mRefractionIndex;

		const Vec3 unitDirection = UnitVector(rayIn.Direction());

		const double cosTheta =
			std::fmin(Dot(-unitDirection, hitRecord.Normal), 1.0);

		const double sinTheta =
			std::sqrt(1.0 - cosTheta * cosTheta);

		const bool cannotRefract =
			refractionRatio * sinTheta > 1.0;

		Vec3 direction;

		if (cannotRefract||Reflectance(cosTheta,refractionRatio)>RandomDouble())
		{
			direction = Reflect(unitDirection, hitRecord.Normal);
		}
		else
		{
			direction = Refract(unitDirection, hitRecord.Normal, refractionRatio);
		}

		scattered = Ray(hitRecord.P, direction);

		return true;

	}

private:
	double mRefractionIndex = 1.0;

	static double Reflectance(double cosine, double refractionIndex)
	{
		//반사율 근사 사용
		auto r0 = (1.0 - refractionIndex) / (1.0 + refractionIndex);
		r0 = r0 * r0;
		return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5);
	}
};