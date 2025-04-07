#include <Math.hpp>

float Math::Mix(float x, float y, float a) {
	return x * (1.0f - a) + y * a;
}
