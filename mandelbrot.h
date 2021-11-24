#include "rgb.h"

// https://en.wikipedia.org/wiki/Mandelbrot_set#Formal_definition
double complex mandelbrot(double complex z, double complex c) {
	return (z*z) + c;
}

size_t mandelbrot_steps_inf(double complex c) {
	const size_t max_radius = 2;	// https://en.wikipedia.org/wiki/Mandelbrot_set#Basic_properties
	const size_t max_iterations = 1000;

	double complex z = 0;
	for (size_t i = 0; i < max_iterations; i++) {
		if (creal(z) > max_radius || cimag(z) > max_radius) {
			return i;
		}

		z = mandelbrot(z, c);
	}

	return 0;
}

// By using a sigmoid function[1] it is possible to map the obtained value
// as an angle (called θ) in the square of color wheel[2], and by using the HSB/HSV
// color model the derived x value (cosine of the angle) can be used as saturation
// and the y value (sine of the angle) as brightness.
//
// Also, when step = 1 the normalized value is 45° making it the darkest color
// possible, leaving the other half of the color wheel left unused. Because of this
// the steps are divided by some constant.
//
// [1]: https://en.wikipedia.org/wiki/Sigmoid_function
// [2]: https://i.ibb.co/60vW99x/any-to-hsb.png
struct rgb mandelbrot_steps_to_rgb(size_t steps) {
	double squished = atan((double) steps/25);	// Squish the step count to the range [0; pi/2)
	struct hsv derived = {235, cos(squished), sin(squished)};
	return rgb_from_hsv(derived);
}
