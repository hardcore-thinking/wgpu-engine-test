#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <cstdint>

#include <Math/Matrix4x4.hpp>
#include <Math/Vector2.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>

namespace Math {
	float Mix(float x, float y, float a);
	uint32_t CeilToNextMultiple(uint32_t value, uint32_t step);
}

#endif // !MATH_CPP