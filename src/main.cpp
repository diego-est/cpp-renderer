/* ========================================================================
 *
 *     Filename:  
 *  Description:  
 *       Author:  
 *      Version:  0.0.1
 *
 * ======================================================================== */
#include <iostream>
#include "utils.hpp"

auto main() -> int
{
	static auto c = Canvas<1000, 1000>(Point(0.0, 0.0), 3.0);

	static auto histogram = [] {
		std::array<Color, 255> ret_arr{};
		const auto begin = Color(182, 0.00, 0.0);
		const auto end = Color(1.0, 1.00, 1.0);
		auto t = 1;
		for (auto & c : ret_arr)
			c = begin.lerp(end, (double)t / 255.0), ++t;

		return ret_arr;
	}();


	[[maybe_unused]] auto mandelbrot = [](Point coordinate) {
		const auto max_iteration = 20000ul;
		auto x = 0.0L;
		auto y = 0.0L;
		auto x_squared = 0.0L;
		auto y_squared = 0.0L;
		auto iteration = 0u;

		while (std::islessequal((x_squared + y_squared), 4) &&
		       (iteration < max_iteration)) {
			y = fma(2 * x, y, coordinate.y);
			x = x_squared - y_squared + coordinate.x;
			x_squared = x * x;
			y_squared = y * y;
			iteration++;
		}

		return Color(iteration);
	};

	auto julia = [](Point z) {
		const  real R = 4;
		auto iteration = 0u;
		const auto max_iteration = 1000u;
		const auto c = Point(-0.8, 0.156);


		while (z.x * z.x + z.y * z.y < R * R  &&  iteration < max_iteration) {
			auto xtemp = z.x * z.x - z.y * z.y;
			z.y = 2 * z.x * z.y  + c.y;
			z.x = xtemp + c.x;
			++iteration;
		}

		return histogram[iteration & 0xFF];
	};

	c.construct(julia);
	c.save_to_ppm("tmp.ppm");

	return 0;
}

