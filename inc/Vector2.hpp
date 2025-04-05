#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <iostream>
#include <iomanip>
#include <cmath>

class Vector2 {
	public:
		Vector2();
		Vector2(float x, float y);
		~Vector2() = default;

	public:
		float X() const;
		float Y() const;

		float U() const;
		float V() const;

		Vector2 operator+(Vector2 const& other) const;
		Vector2 operator-(Vector2 const& other) const;
		Vector2 operator-() const;
		Vector2 operator*(float scalar) const;
		Vector2 operator/(float scalar) const;

		friend std::ostream& operator<<(std::ostream& os, Vector2 const& vec);

		float Magnitude() const;

		static Vector2 Normalize(Vector2 const& vec);
		static float Dot(Vector2 const& a, Vector2 const& b);

	private:
		float _x = 0.0f;
		float _y = 0.0f;
};

#endif // !VECTOR2_HPP
