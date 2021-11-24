#include <stdio.h>
#include <stdbool.h>
#include <complex.h>
#include <string.h>
#include <stdint.h>
#include "mandelbrot.h"


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
		fprintf(output, "P6\n%lu %lu\n255\n", width, height);	// PPM format (https://en.wikipedia.org/wiki/Netpbm#Description)
	}

	double r_step = (creal(bottom_right) - creal(top_left)) / (double) width;
	double i_step = (cimag(top_left) - cimag(bottom_right)) / (double) height;

	for (double i = cimag(top_left), y = 0; y < height; i -= i_step, y++) {
		for (double r = creal(top_left), x = 0; x < width; r += r_step, x++) {
			double complex point = r + i*I;
			bool inside = mandelbrot_steps_to_inf(point) == 0;

			if (is_output_tty) {
				fprintf(output, "%s", inside ? "*" : " ");
			} else {
				if (!inside) {
					fwrite((uint8_t[]) {255, 255, 255}, 1, 3, output);
				} else {
					fwrite((uint8_t[]) {0, 0, 0}, 1, 3, output);
				}
			}
		}

		if (is_output_tty) {
			fprintf(output, "\n");
		}
	}

	fclose(output);
	return 0;
}
