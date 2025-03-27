#include <Vec4.hpp>

Vec4::Vec4() {
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
	_w = 0.0f;
}

Vec4::Vec4(float x, float y, float z, float w) {
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

float Vec4::X() const {
	return _x;
}

float Vec4::Y() const {
	return _y;
}

float Vec4::Z() const {
	return _z;
}

float Vec4::W() const {
	return _w;
}

float Vec4::R() const {
	return _x;
}

float Vec4::G() const {
	return _y;
}

float Vec4::B() const {
	return _z;
}

float Vec4::A() const {
	return _w;
}

Vec4 Vec4::operator+(Vec4 const& other) const {
	return Vec4(_x + other._x, _y + other._y, _z + other._z, _w + other._w);
}

Vec4 Vec4::operator-(Vec4 const& other) const {
	return Vec4(_x - other._x, _y - other._y, _z - other._z, _w - other._w);
}

Vec4 Vec4::operator*(float scalar) const {
	return Vec4(_x * scalar, _y * scalar, _z * scalar, _w * scalar);
}

Vec4 Vec4::operator/(float scalar) const {
	return Vec4(_x / scalar, _y / scalar, _z / scalar, _w / scalar);
}

float Vec4::Dot(Vec4 const& a, Vec4 const& b) {
	return a._x * b._x + a._y * b._y + a._z * b._z + a._w * b._w;
}
