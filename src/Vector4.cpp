#include <Vector4.hpp>

Vector4::Vector4() {
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
	_w = 0.0f;
}

Vector4::Vector4(float x, float y, float z, float w) {
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

float Vector4::X() const {
	return _x;
}

float Vector4::Y() const {
	return _y;
}

float Vector4::Z() const {
	return _z;
}

float Vector4::W() const {
	return _w;
}

float Vector4::R() const {
	return _x;
}

float Vector4::G() const {
	return _y;
}

float Vector4::B() const {
	return _z;
}

float Vector4::A() const {
	return _w;
}

Vector4 Vector4::operator+(Vector4 const& other) const {
	return Vector4(_x + other._x, _y + other._y, _z + other._z, _w + other._w);
}

Vector4 Vector4::operator-(Vector4 const& other) const {
	return Vector4(_x - other._x, _y - other._y, _z - other._z, _w - other._w);
}

Vector4 Vector4::operator*(float scalar) const {
	return Vector4(_x * scalar, _y * scalar, _z * scalar, _w * scalar);
}

Vector4 Vector4::operator/(float scalar) const {
	return Vector4(_x / scalar, _y / scalar, _z / scalar, _w / scalar);
}

float Vector4::Dot(Vector4 const& a, Vector4 const& b) {
	return a._x * b._x + a._y * b._y + a._z * b._z + a._w * b._w;
}
