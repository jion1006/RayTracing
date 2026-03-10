#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include<cmath>
#include<iostream>
#include<limits>
#include<memory>
#include<vector>
#include<cstdlib>

using std::make_shared;
using std::shared_ptr;

constexpr double Infinity = std::numeric_limits<double>::infinity();
constexpr double Pi = 3.1415926535897932385;

inline double DegreesToRadians(double degrees)
{
	return degrees * Pi / 180.0;
}

inline double RandomDouble()
{
	return std::rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double minimum, double maximum)
{
	return minimum + (maximum - minimum) * RandomDouble();
}

#include"Color.h"
#include"Ray.h"
#include"Vec3.h"
#include"Interval.h"

#endif // !RTWEEKEND_H
