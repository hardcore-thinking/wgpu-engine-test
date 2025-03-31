#include <Vector3.hpp>

Vector3::Vector3() {
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
}

Vector3::Vector3(float x, float y, float z) {
	_x = x;
	_y = y;
	_z = z;
}

float Vector3::X() const {
	return _x;
}

float Vector3::Y() const {
	return _y;
}

float Vector3::Z() const {
	return _z;
}

float Vector3::R() const {
	return _x;
}

float Vector3::G() const {
	return _y;
}

float Vector3::B() const {
	return _z;
}

Vector3 Vector3::operator+(Vector3 const& other) const {
	return Vector3(_x + other._x, _y + other._y, _z + other._z);
}

Vector3 Vector3::operator-(Vector3 const& other) const {
	return Vector3(_x - other._x, _y - other._y, _z - other._z);
}

Vector3 Vector3::operator*(float scalar) const {
	return Vector3(_x * scalar, _y * scalar, _z * scalar);
}

Vector3 Vector3::operator/(float scalar) const {
	return Vector3(_x / scalar, _y / scalar, _z / scalar);
}

float Vector3::Dot(Vector3 const& a, Vector3 const& b) {
	return a._x * b._x + a._y * b._y + a._z * b._z;
}
