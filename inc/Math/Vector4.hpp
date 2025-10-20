#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <iostream>
#include <iomanip>
#include <cmath>

#include <Math/Vector2.hpp>
#include <Math/Vector3.hpp>

namespace Math {
	struct Vector4 {
	public:
		Vector4();
		Vector4(float c);
		Vector4(float x, float y, float z, float w);
		Vector4(Vector2 const& xy, float z, float w);
		Vector4(float x, Vector2 const& yz, float w);
		Vector4(float x, float y, Vector2 const& zw);
		Vector4(Vector2 const& xy, Vector2 const& zw);
		Vector4(Vector3 const& xyz, float w);
		Vector4(float x, Vector3 const& yzw);

		Vector4(Vector4 const& m) = default;
		Vector4(Vector4&& m) = default;

		Vector4& operator = (Vector4 const& m);

		friend bool operator == (Vector4 const& lhs, Vector4 const& rhs);
		friend bool operator != (Vector4 const& lhs, Vector4 const& rhs);

		friend std::ostream& operator << (std::ostream& out, Vector4 const& m);

		friend Vector4 operator + (Vector4 const& lhs, Vector4 const& rhs);
		friend Vector4& operator += (Vector4& lhs, Vector4 const& rhs);

		friend Vector4 operator - (Vector4 const& m);
		friend Vector4 operator - (Vector4 const& lhs, Vector4 const& rhs);
		friend Vector4& operator -= (Vector4& lhs, Vector4 const& rhs);

		friend Vector4 operator * (Vector4 const& lhs, float const& rhs);
		friend Vector4 operator * (float const& lhs, Vector4 const& rhs);
		friend Vector4& operator *= (Vector4& lhs, float const& rhs);

		friend Vector4 operator / (Vector4 const& lhs, float const& rhs);
		friend Vector4& operator /= (Vector4& lhs, float const& rhs);

		static float Dot(Vector4 const& lhs, Vector4 const& rhs);
		static float Magnitude(Vector4 const& m);
		static Vector4 Normalize(Vector4 const& m);

		[[nodiscard]] Vector4& Normalized();

	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;
	};
}

#endif // VECTOR4_HPP
