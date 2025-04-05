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

Vector3 Vector3::operator-() const {
	return Vector3(-_x, -_y, -_z);
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

std::ostream& operator<<(std::ostream& os, Vector3 const& vec) {
	std::ios::fmtflags f(os.flags());
	os << std::showpos << std::setprecision(3) << std::fixed;
	os << "|" << vec.X() << ", " << vec.Y() << ", " << vec.Z() << "|";
	os.flags(f);
	return os;
}

float Vector3::Magnitude() const {
	return std::sqrtf((_x * _x) + (_y * _y) + (_z * _z));
}

Vector3 Vector3::Normalize(Vector3 const& vec) {
	float magnitude = vec.Magnitude();
	if (magnitude == 0.0f) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	float magnitudeInverse = 1 / magnitude;
	return Vector3(vec.X() * magnitudeInverse, vec.Y() * magnitudeInverse, vec.Z() * magnitudeInverse);
}

float Vector3::Dot(Vector3 const& a, Vector3 const& b) {
	return a._x * b._x + a._y * b._y + a._z * b._z;
}

Vector3 Vector3::Cross(Vector3 const& a, Vector3 const& b) {
	return Vector3(
		a._y * b._z - a._z * b._y,
		a._z * b._x - a._x * b._z,
		a._x * b._y - a._y * b._x
	);
}

