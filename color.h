#ifndef COLOR_H
#define COLOR_H


#include"Interval.h"
#include "Vec3.h"

using Color = Vec3;

inline double LinearToGamma(double linearComponent)
{
	if (linearComponent > 0.0)
	{
		return std::sqrt(linearComponent);
	}
	return 0.0;
}

void WriteColor(std::ostream& out, const Color& pixelColor)
{
	auto r = pixelColor.X();
	auto g = pixelColor.Y();
	auto b = pixelColor.Z();

	r = LinearToGamma(r);
	g = LinearToGamma(g);
	b = LinearToGamma(b);

	static const Interval intensity(0.000, 0.999);


	int rByte = static_cast<int>(256.0 * intensity.Clamp(r));
	int gByte = static_cast<int>(256.0 * intensity.Clamp(g));
	int bByte = static_cast<int>(256.0 * intensity.Clamp(b));

	out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

#endif