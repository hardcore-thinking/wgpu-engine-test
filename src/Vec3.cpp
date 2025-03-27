#include <Vec3.hpp>

Vec3::Vec3() {
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
}

Vec3::Vec3(float x, float y, float z) {
	_x = x;
	_y = y;
	_z = z;
}

float Vec3::X() const {
	return _x;
}

float Vec3::Y() const {
	return _y;
}

float Vec3::Z() const {
	return _z;
}

float Vec3::R() const {
	return _x;
}

float Vec3::G() const {
	return _y;
}

float Vec3::B() const {
	return _z;
}

Vec3 Vec3::operator+(Vec3 const& other) const {
	return Vec3(_x + other._x, _y + other._y, _z + other._z);
}

Vec3 Vec3::operator-(Vec3 const& other) const {
	return Vec3(_x - other._x, _y - other._y, _z - other._z);
}

Vec3 Vec3::operator*(float scalar) const {
	return Vec3(_x * scalar, _y * scalar, _z * scalar);
}

Vec3 Vec3::operator/(float scalar) const {
	return Vec3(_x / scalar, _y / scalar, _z / scalar);
}

float Vec3::Dot(Vec3 const& a, Vec3 const& b) {
	return a._x * b._x + a._y * b._y + a._z * b._z;
}
