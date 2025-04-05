#include <Matrix4x4.hpp>

Matrix4x4::Matrix4x4() {
	_elements.fill(0.0f);
}

Matrix4x4::Matrix4x4(float x0, float y0, float z0, float w0,
	float x1, float y1, float z1, float w1,
	float x2, float y2, float z2, float w2,
	float x3, float y3, float z3, float w3) {
	_elements = {
		x0, y0, z0, w0,
		x1, y1, z1, w1,
		x2, y2, z2, w2,
		x3, y3, z3, w3
	};
}

Matrix4x4 Matrix4x4::Identity() {
	Matrix4x4 result;
	result._elements = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}

Matrix4x4 Matrix4x4::Transpose(Matrix4x4 const& mat) {
	Matrix4x4 result;
	std::array<float, 16> const& e = mat._elements;

	result._elements = {
		e[0], e[4],  e[8], e[12],
		e[1], e[5],  e[9], e[13],
		e[2], e[6], e[10], e[14],
		e[3], e[7], e[11], e[15],
	};

	return result;
}

Matrix4x4 Matrix4x4::RotateX(float angle) {
	float c = std::cos(angle);
	float s = std::sin(angle);

	return Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f,    c,   -s, 0.0f,
		0.0f,    s,    c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::RotateY(float angle) {
	float c = std::cos(angle);
	float s = std::sin(angle);

	return Matrix4x4(
		   c, 0.0f,    s, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
	   	  -s, 0.0f,    c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::RotateZ(float angle) {
	float c = std::cos(angle);
	float s = std::sin(angle);

	return Matrix4x4(
		   c,   -s, 0.0f, 0.0f,
		   s,    c, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::Translate(float tx, float ty, float tz) {
	return Matrix4x4(
		1.0f, 0.0f, 0.0f,   tx,
		0.0f, 1.0f, 0.0f,   ty,
		0.0f, 0.0f, 1.0f,   tz,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::Scale(float s) {
	return Matrix4x4(
		   s, 0.0f, 0.0f, 0.0f,
		0.0f,    s, 0.0f, 0.0f,
		0.0f, 0.0f,    s, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::Scale(float sx, float sy, float sz) {
	return Matrix4x4(
		  sx, 0.0f, 0.0f, 0.0f,
		0.0f,   sy, 0.0f, 0.0f,
		0.0f, 0.0f,   sz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::Perspective(float fov, float ratio, float near, float far) {
	float focalLength = 1.0f / std::tan(fov / 2.0f);
	float divider = 1.0f / (far - near);
	return Matrix4x4(
		focalLength / ratio,        0.0f,          0.0f,                  0.0f,
		               0.0f, focalLength,          0.0f,                  0.0f,
		               0.0f,        0.0f, far * divider, -far * near * divider,
		               0.0f,        0.0f,          1.0f,                  0.0f
	);
}

Matrix4x4 Matrix4x4::Orthographic(float ratio, float near, float far) {
	float divider = 1.0f / (far - near);
	return Matrix4x4(
		1.0f,  0.0f,           0.0f,              0.0f,
		0.0f, ratio,           0.0f,              0.0f,
		0.0f,  0.0f, 1.0f * divider, (-near) * divider,
		0.0f,  0.0f,           0.0f,              1.0f
	);
}

Matrix4x4 Matrix4x4::LookAt(Vector3 const& from, Vector3 const& to, Vector3 const& upDirection) {
	Vector3 forward = Vector3::Normalize(to - from);
	Vector3 right = Vector3::Normalize(Vector3::Cross(upDirection, forward));
	Vector3 up = Vector3::Cross(forward, right);

	Vector3 translation(
		-Vector3::Dot(right, from),
		-Vector3::Dot(up, from),
		-Vector3::Dot(forward, from)
	);

	return Matrix4x4(
		  right.X(),   right.Y(),   right.Z(), translation.X(),
	 	     up.X(),      up.Y(),      up.Z(), translation.Y(),
		forward.X(), forward.Y(), forward.Z(), translation.Z(),
		       0.0f,        0.0f,        0.0f,            1.0f
	);
}

float Matrix4x4::Element(int n, int m) const {
	return _elements[n * 4 + m];
}

std::array<float, 16> Matrix4x4::Elements() const {
	return _elements;
}

Vector4 Matrix4x4::Line(int n) const {
	return Vector4(_elements[n * 4 + 0], _elements[n * 4 + 1], _elements[n * 4 + 2], _elements[n * 4 + 3]);
}

Vector4 Matrix4x4::Column(int m) const {
	return Vector4(_elements[0 * 4 + m], _elements[1 * 4 + m], _elements[2 * 4 + m], _elements[3 * 4 + m]);
}

Matrix4x4 Matrix4x4::operator+(Matrix4x4 const& other) const {
	Matrix4x4 result;
	for (int i = 0; i < 16; ++i) {
		result._elements[i] = _elements[i] + other._elements[i];
	}
	return result;
}

Matrix4x4 Matrix4x4::operator-() const {
	Matrix4x4 result;
	for (int i = 0; i < 16; ++i) {
		result._elements[i] = -_elements[i];
	}
	return result;
}

Matrix4x4 Matrix4x4::operator-(Matrix4x4 const& other) const {
	return *this + -other;
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4 const& other) const {
	Matrix4x4 result;

	result._elements[0]  = Element(0, 0) * other.Element(0, 0) + Element(0, 1) * other.Element(1, 0) + Element(0, 2) * other.Element(2, 0) + Element(0, 3) * other.Element(3, 0);
	result._elements[1]  = Element(0, 0) * other.Element(0, 1) + Element(0, 1) * other.Element(1, 1) + Element(0, 2) * other.Element(2, 1) + Element(0, 3) * other.Element(3, 1);
	result._elements[2]  = Element(0, 0) * other.Element(0, 2) + Element(0, 1) * other.Element(1, 2) + Element(0, 2) * other.Element(2, 2) + Element(0, 3) * other.Element(3, 2);
	result._elements[3]  = Element(0, 0) * other.Element(0, 3) + Element(0, 1) * other.Element(1, 3) + Element(0, 2) * other.Element(2, 3) + Element(0, 3) * other.Element(3, 3);

	result._elements[4]  = Element(1, 0) * other.Element(0, 0) + Element(1, 1) * other.Element(1, 0) + Element(1, 2) * other.Element(2, 0) + Element(1, 3) * other.Element(3, 0);
	result._elements[5]  = Element(1, 0) * other.Element(0, 1) + Element(1, 1) * other.Element(1, 1) + Element(1, 2) * other.Element(2, 1) + Element(1, 3) * other.Element(3, 1);
	result._elements[6]  = Element(1, 0) * other.Element(0, 2) + Element(1, 1) * other.Element(1, 2) + Element(1, 2) * other.Element(2, 2) + Element(1, 3) * other.Element(3, 2);
	result._elements[7]  = Element(1, 0) * other.Element(0, 3) + Element(1, 1) * other.Element(1, 3) + Element(1, 2) * other.Element(2, 3) + Element(1, 3) * other.Element(3, 3);

	result._elements[8]  = Element(2, 0) * other.Element(0, 0) + Element(2, 1) * other.Element(1, 0) + Element(2, 2) * other.Element(2, 0) + Element(2, 3) * other.Element(3, 0);
	result._elements[9]  = Element(2, 0) * other.Element(0, 1) + Element(2, 1) * other.Element(1, 1) + Element(2, 2) * other.Element(2, 1) + Element(2, 3) * other.Element(3, 1);
	result._elements[10] = Element(2, 0) * other.Element(0, 2) + Element(2, 1) * other.Element(1, 2) + Element(2, 2) * other.Element(2, 2) + Element(2, 3) * other.Element(3, 2);
	result._elements[11] = Element(2, 0) * other.Element(0, 3) + Element(2, 1) * other.Element(1, 3) + Element(2, 2) * other.Element(2, 3) + Element(2, 3) * other.Element(3, 3);

	result._elements[12] = Element(3, 0) * other.Element(0, 0) + Element(3, 1) * other.Element(1, 0) + Element(3, 2) * other.Element(2, 0) + Element(3, 3) * other.Element(3, 0);
	result._elements[13] = Element(3, 0) * other.Element(0, 1) + Element(3, 1) * other.Element(1, 1) + Element(3, 2) * other.Element(2, 1) + Element(3, 3) * other.Element(3, 1);
	result._elements[14] = Element(3, 0) * other.Element(0, 2) + Element(3, 1) * other.Element(1, 2) + Element(3, 2) * other.Element(2, 2) + Element(3, 3) * other.Element(3, 2);
	result._elements[15] = Element(3, 0) * other.Element(0, 3) + Element(3, 1) * other.Element(1, 3) + Element(3, 2) * other.Element(2, 3) + Element(3, 3) * other.Element(3, 3);

	return result;
}

std::ostream& operator<<(std::ostream& os, Matrix4x4 const& mat) {
	std::ios_base::fmtflags f(std::cout.flags());

	for (int i = 0; i < 4; ++i) {
		os << "|";
		for (int j = 0; j < 4; ++j) {
			os << std::showpos << std::setprecision(3) << std::fixed << mat.Element(i, j);
			if (j < 3) {
				os << ", ";
			}
		}

		os << "|";
		std::cout << std::endl;
	}

	std::cout.flags(f);

	return os;
}