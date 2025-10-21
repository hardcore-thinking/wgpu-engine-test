#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <iostream>
#include <iomanip>
#include <cmath>

namespace Math {
	struct Vector2 {
	public:
		Vector2();
		Vector2(float c);
		Vector2(float x, float y);
		Vector2(Vector2 const& m) = default;
		Vector2(Vector2&& m) = default;

		Vector2& operator = (Vector2 const& m);

		friend bool operator == (Vector2 const& lhs, Vector2 const& rhs);
		friend bool operator != (Vector2 const& lhs, Vector2 const& rhs);

		friend std::ostream& operator << (std::ostream& out, Vector2 const& m);

		friend Vector2 operator + (Vector2 const& lhs, Vector2 const& rhs);
		friend Vector2& operator += (Vector2& lhs, Vector2 const& rhs);

		friend Vector2 operator - (Vector2 const& m);
		friend Vector2 operator - (Vector2 const& lhs, Vector2 const& rhs);
		friend Vector2& operator -= (Vector2& lhs, Vector2 const& rhs);

		friend Vector2 operator * (Vector2 const& lhs, float const& rhs);
		friend Vector2 operator * (float const& lhs, Vector2 const& rhs);
		friend Vector2& operator *= (Vector2& lhs, float const& rhs);

		friend Vector2 operator / (Vector2 const& lhs, float const& rhs);
		friend Vector2& operator /= (Vector2& lhs, float const& rhs);

		static float Dot(Vector2 const& lhs, Vector2 const& rhs);
		static float Magnitude(Vector2 const& m);
		static Vector2 Normalize(Vector2 const& m);

		[[nodiscard]] Vector2& Normalized();

	public:
		float x = 0.0f;
		float y = 0.0f;
	};
}

#endif // VECTOR2_HPP
