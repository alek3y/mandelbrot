#include <stdio.h>
#include <stdbool.h>
#include <complex.h>

// https://en.wikipedia.org/wiki/Mandelbrot_set#Formal_definition
double complex mandelbrot(double complex z, double complex c) {
	return (z*z) + c;
}

bool is_inside_mandelbrot(double complex c) {
	const size_t max_radius = 2;	// https://en.wikipedia.org/wiki/Mandelbrot_set#Basic_properties
	const size_t max_iterations = 1000;

	double complex z = 0;
	for (size_t i = 0; i < max_iterations; i++) {
		if (creal(z) > max_radius || cimag(z) > max_radius) {
			return false;
		}

		z = mandelbrot(z, c);
	}

	return true;
}

#define RESOLUTION 100
#define HEIGHT_SCALE 1/2.5

int main() {
	const double complex top_left = -2 + 1*I;
	const double complex bottom_right = 0.5 + -1*I;

	double r_step = (creal(bottom_right) - creal(top_left)) / ((double) RESOLUTION);
	double i_step = (cimag(top_left) - cimag(bottom_right)) / ((double) RESOLUTION * HEIGHT_SCALE);

	for (double i = cimag(top_left); i > cimag(bottom_right); i -= i_step) {
		for (double r = creal(top_left); r < creal(bottom_right); r += r_step) {
			double complex point = r + i*I;

			printf("%s",
				is_inside_mandelbrot(point) ? "*" : " "
			);
		}
		printf("\n");
	}
}
