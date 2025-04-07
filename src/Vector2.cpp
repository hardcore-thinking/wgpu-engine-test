#include <Vector2.hpp>

namespace Math {
	Vector2::Vector2() {
		_x = 0.0f;
		_y = 0.0f;
	}

	Vector2::Vector2(float c) {
		_x = c;
		_y = c;
	}

	Vector2::Vector2(float x, float y) {
		_x = x;
		_y = y;
	}

	Vector2 Vector2::operator+(Vector2 const& other) const {
		return Vector2(_x + other._x, _y + other._y);
	}

	Vector2 Vector2::operator-() const {
		return Vector2(-_x, -_y);
	}

	Vector2 Vector2::operator-(Vector2 const& other) const {
		return Vector2(_x - other._x, _y - other._y);
	}

	Vector2 Vector2::operator*(float scalar) const {
		return Vector2(_x * scalar, _y * scalar);
	}

	Vector2 Vector2::operator/(float scalar) const {
		return Vector2(_x / scalar, _y / scalar);
	}

	std::ostream& operator<<(std::ostream& os, Vector2 const& vec) {
		std::ios::fmtflags f(os.flags());
		os << std::showpos << std::setprecision(3) << std::fixed;
		os << "|" << vec.X() << ", " << vec.Y() << "|";
		os.flags(f);
		return os;
	}

	float Vector2::Magnitude() const {
		return std::sqrtf((_x * _x) + (_y * _y));
	}

	Vector2 Vector2::Normalize(Vector2 const& vec) {
		float magnitude = vec.Magnitude();
		if (magnitude == 0.0f) {
			return Vector2(0.0f, 0.0f);
		}

		float magnitudeInverse = 1 / magnitude;
		return Vector2(vec.X() * magnitudeInverse, vec.Y() * magnitudeInverse);
	}

	float Vector2::Dot(Vector2 const& a, Vector2 const& b) {
		return a._x * b._x + a._y * b._y;
	}
}