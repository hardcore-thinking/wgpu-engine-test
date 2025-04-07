#include <Vector4.hpp>

namespace Math {
	Vector4::Vector4() {
		_x = 0.0f;
		_y = 0.0f;
		_z = 0.0f;
		_w = 0.0f;
	}

	Vector4::Vector4(float c) {
		_x = c;
		_y = c;
		_z = c;
		_w = c;
	}

	Vector4::Vector4(float x, float y, float z, float w) {
		_x = x;
		_y = y;
		_z = z;
		_w = w;
	}

	Vector4 Vector4::operator+(Vector4 const& other) const {
		return Vector4(_x + other._x, _y + other._y, _z + other._z, _w + other._w);
	}

	Vector4 Vector4::operator-(Vector4 const& other) const {
		return Vector4(_x - other._x, _y - other._y, _z - other._z, _w - other._w);
	}

	Vector4 Vector4::operator-() const {
		return Vector4(-_x, -_y, -_z, -_w);
	}

	Vector4 Vector4::operator*(float scalar) const {
		return Vector4(_x * scalar, _y * scalar, _z * scalar, _w * scalar);
	}

	Vector4 Vector4::operator/(float scalar) const {
		return Vector4(_x / scalar, _y / scalar, _z / scalar, _w / scalar);
	}

	std::ostream& operator<<(std::ostream& os, Vector4 const& vec) {
		std::ios::fmtflags f(os.flags());
		os << std::showpos << std::setprecision(3) << std::fixed;
		os << "|" << vec.X() << ", " << vec.Y() << ", " << vec.Z() << ", " << vec.W() << "|";
		os.flags(f);
		return os;
	}


	float Vector4::Magnitude() const {
		return std::sqrt(_x * _x + _y * _y + _z * _z + _w * _w);
	}

	float Vector4::Dot(Vector4 const& a, Vector4 const& b) {
		return a._x * b._x + a._y * b._y + a._z * b._z + a._w * b._w;
	}
}