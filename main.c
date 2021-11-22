#include <stdio.h>
#include <stdbool.h>
#include <complex.h>
#include <string.h>

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

#define DEFAULT_RES_WIDTH 100
#define DEFAULT_RES_HEIGHT 40

int main(int argc, char *argv[]) {
	FILE *output = stdout;
	size_t width = DEFAULT_RES_WIDTH, height = DEFAULT_RES_HEIGHT;
	for (int i = 0; i < argc; i++) {
		char *arg = argv[i];

		if (strcmp(arg, "-r") == 0) {
			char *param = argv[++i];

			size_t arg_width = 0, arg_height = 0;
			sscanf(param, "%lu,%lu", &arg_width, &arg_height);

			width = arg_width;
			height = arg_height ? arg_height : arg_width;
		} else if (strcmp(arg, "-o") == 0) {
			char *param = argv[++i];
			if (strcmp(param, "-") != 0) {
				output = fopen(param, "w");
			}
		} else if (i > 0) {
			fprintf(stderr,
				"Usage: %s [OPTION]\n"
				"Generate the mandelbrot set.\n\n"
				"OPTIONS:\n"
				" -r width,height       specify output resolution\n"
				" -o filename           write to PBM image instead of terminal\n",
				argv[0]
			);

			return 0;
		}
	}

	const double complex top_left = -2 + 1*I;
	const double complex bottom_right = 0.5 + -1*I;

	bool is_output_tty = output == stdout || output == stderr;

	if (!is_output_tty) {
		fprintf(output, "P1\n%lu %lu\n", width, height);	// PBM format (https://en.wikipedia.org/wiki/Netpbm#Description)
	}

	double r_step = (creal(bottom_right) - creal(top_left)) / (double) width;
	double i_step = (cimag(top_left) - cimag(bottom_right)) / (double) height;

	for (double i = cimag(top_left), y = 0; y < height; i -= i_step, y++) {
		for (double r = creal(top_left), x = 0; x < width; r += r_step, x++) {
			double complex point = r + i*I;
			bool inside = is_inside_mandelbrot(point);

			fprintf(output, "%s",
				is_output_tty ? (inside ? "*" : " ") : (inside ? "1" : "0")
			);
		}
		fprintf(output, "\n");
	}

	fclose(output);
	return 0;
}
