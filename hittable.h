#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include"Color.h"

class Material;

class HitRecord
{
public:
	void SetFaceNormal(const Ray& r, const Vec3& outwardNormal)
	{
		bFrontFace = Dot(r.Direction(), outwardNormal) < 0;
		Normal = bFrontFace ? outwardNormal : -outwardNormal;
	}
	Point3 P;
	Vec3 Normal;
	double T = 0.0;
	bool bFrontFace = false;
	std::shared_ptr<Material> material;
};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool Hit(const Ray& r, const Interval& rayT, HitRecord& rec) const = 0;
};
#endif 
