#include <array>
#include <cstdint>
#include <fstream>
#include <cmath>
using size_t = std::size_t;
using real = long double;

struct RGB {
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;

	RGB(uint8_t r, uint8_t g, uint8_t b);
};

struct Color {
	double hue;
	double saturation;
	double value;
	Color();
	Color(uint32_t val);
	Color(double hue, double saturation, double value);

	auto to_rgb() const -> RGB;
	auto lerp(const Color&, double) const -> Color;
};

struct Point {
	real x;
	real y;

	Point(real x, real y);
};

template<size_t width, size_t height>
class Canvas {
	real x_max;
	real y_max;
	real x_min;
	real y_min;
	std::array<Color, width * height> pixels;

	public:
	Canvas(real min_x, real min_y, real max_x, real max_y)
		: x_max(max_x)
		, y_max(max_y)
		, x_min(min_x)
		, y_min(min_y)
	{  }

	Canvas(Point p, size_t size)
	{
		const auto delta = size / 2.0;
		x_max = p.x + delta;
		x_min = p.x - delta;
		y_max = p.y + delta;
		y_min = p.y - delta;
	}

	auto save_to_ppm(const char* file_path) const -> void
	{
		auto f =
		    std::fstream(file_path, std::ios::binary | std::ios::out);
		f << "P6\n" << width << " " << height << " 255\n";
		for (auto const& color : pixels) {
			auto c = color.to_rgb();
			uint8_t rgb[3] = {c.Red, c.Green, c.Blue};
			f << rgb[0]  << rgb[1]  << rgb[2] ;
		}
		f.close();
	}

	auto coordinate(size_t index) const -> Point
	{
		return Point(
		    std::lerp(x_min, x_max,
			      (index % width) / (real)width),
		    std::lerp(y_min, y_max,
			      ((real)index / height) / height));
	}

	auto construct(auto op) -> void
	{
		#pragma omp parallel for
		for (size_t idx = 0; idx < width * height; idx++)
			pixels[idx] = op(coordinate(idx));
	}

	auto operator[](size_t index) -> Color&
	{
		return pixels[index];
	}

	auto operator()(size_t row, size_t col) -> Color&
	{
		return pixels[row * width + col];
	}
};
