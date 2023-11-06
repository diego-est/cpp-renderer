#include "utils.hpp"

RGB::RGB(uint8_t r, uint8_t g, uint8_t b)
	: Red(r)
	, Green(g)
	, Blue(b)
{
}

Color::Color()
	: hue(0.0)
	, saturation(1.0)
	, value(1.0)
{
}

Color::Color(uint32_t val)
	: hue(0.0)
	, saturation(0.0)
{
	value = std::min((double) val, 255.0);
}

Color::Color(double hue, double saturation, double value)
	: hue(hue)
	, saturation(saturation)
	, value(value)
{
}

auto Color::to_rgb() const -> RGB
{
	const auto chroma = saturation * value;
	const auto cube_hue = hue / 60.0;
	const auto tmp = chroma * (1.0 - fabs(fmod(cube_hue, 2.0) - 1.0));

	auto components = std::array{ 0.0, 0.0, 0.0 };
	if (cube_hue < 1.0)
		components = { chroma, tmp, 0.0 };
	else if (cube_hue < 2.0)
		components = { tmp, chroma, 0.0 };
	else if(cube_hue < 3.0)
		components = { 0.0, chroma, tmp };
	else if(cube_hue < 4.0)
		components = { 0.0, tmp, chroma };
	else if(cube_hue < 5.0)
		components = { tmp, 0.0, chroma };
	else
		components = { chroma, 0.0, tmp };

	const auto match = value - chroma;
	for (auto& component : components)
		component += match, component *= 255;

	const auto c = RGB(round(components[0]), round(components[1]), round(components[2]));

	return c;
}

auto Color::lerp(const Color& rhs, double t) const -> Color
{
	return Color(std::lerp(hue, rhs.hue, t),
		     std::lerp(saturation, rhs.saturation, t),
		     std::lerp(value, rhs.value, t)
		    );
}

Point::Point(real x, real y)
	: x(x)
	, y(y)
{
}
