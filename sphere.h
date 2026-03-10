#ifndef SPHERE_H
#define SPHERE_H

#include"Hittable.h"
#include"Vec3.h"
#include<memory>

class Sphere :public Hittable
{
public:
	Sphere(const Point3& center, double radius,const std::shared_ptr<Material>& material)
		:mCenter(center)
		,mRadius(std::fmax(0.0,radius))
		,mMaterial(material)
	{
	}

	bool Hit(
		const Ray& ray,
		const Interval& rayT,
		HitRecord& hitRecord
	) const override
	{
		Vec3 originToCenter = mCenter - ray.Origin();

		auto a = ray.Direction().LengthSquared();
		auto h = Dot(ray.Direction(), originToCenter);
		auto c = originToCenter.LengthSquared() - mRadius * mRadius;

		auto discriminant = h * h - a * c;
		if (discriminant < 0.0)
		{
			return false;
		}
		auto squareRootDiscriminant = std::sqrt(discriminant);

		auto root = (h - squareRootDiscriminant) / a;
		if (!rayT.Surrounds(root))
		{
			root = (h + squareRootDiscriminant) / a;
			if (!rayT.Surrounds(root))
			{
				return false;
			}
		}

		hitRecord.T = root;
		hitRecord.P = ray.At(hitRecord.T);

		Vec3 outwardNormal=(hitRecord.P - mCenter) / mRadius;
		hitRecord.SetFaceNormal(ray, outwardNormal);

		hitRecord.material = mMaterial;
		return true;
	}

private:
	Point3 mCenter;
	double mRadius;
	std::shared_ptr<Material>mMaterial;
	
};

#endif
