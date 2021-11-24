struct rgb {
	uint8_t r, g, b;
};

struct hsv {
	double h, s, v;
};

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

// I don't know how tf this is supposed to work, but props to Wikipedia:
// https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB_alternative
double rgb_from_hsv_f(double n, struct hsv color) {
	double k = fmod(n + color.h/60, 6);
	return color.v - (color.v * color.s) * MAX(0, MIN(MIN(k, 4 - k), 1));
}

struct rgb rgb_from_hsv(struct hsv hsv_color) {
	struct rgb rgb_color;
	rgb_color.r = rgb_from_hsv_f(5, hsv_color) * 255;
	rgb_color.g = rgb_from_hsv_f(3, hsv_color) * 255;
	rgb_color.b = rgb_from_hsv_f(1, hsv_color) * 255;

	return rgb_color;
}
