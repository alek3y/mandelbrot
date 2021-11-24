
// https://en.wikipedia.org/wiki/Mandelbrot_set#Formal_definition
double complex mandelbrot(double complex z, double complex c) {
	return (z*z) + c;
}

size_t mandelbrot_steps_to_inf(double complex c) {
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
