#include <Math/Math.hpp>

namespace Math {
	float Mix(float x, float y, float a) {
		return x * (1.0f - a) + y * a;
	}

	uint32_t CeilToNextMultiple(uint32_t value, uint32_t step) {
		uint32_t divideAndCeil = value / step + (value % step == 0 ? 0 : 1);
		return step * divideAndCeil;
	}
}
