#ifndef MATRIX4X4_HPP
#define MATRIX4X4_HPP

#include <iostream>
#include <array>
#include <iomanip>

#include <Vec4.hpp>

// this Matrix4x4 class uses the row major order
class Matrix4x4 {
	public:
		Matrix4x4();
		Matrix4x4(float x0, float y0, float z0, float w0,
			      float x1, float y1, float z1, float w1,
			      float x2, float y2, float z2, float w2,
			      float x3, float y3, float z3, float w3)
		~Matrix4x4() = default;

	public:
		static Matrix4x4 Identity();
		static Matrix4x4 Transpose(Matrix4x4 const& mat);

		static Matrix4x4 RotateX(float angle);
		static Matrix4x4 RotateY(float angle);
		static Matrix4x4 RotateZ(float angle);

		static Matrix4x4 Translate(float tx, float ty, float tz);

		static Matrix4x4 Scale(float s);
		static Matrix4x4 Scale(float sx, float sy, float sz);

		static Matrix4x4 Perspective(float fov, float ratio, float near, float far);
		static Matrix4x4 Orthographic(float ratio, float near, float far);

		float Element(int n, int m) const;
		std::array<float, 16> Elements() const;
		Vec4 Line(int n) const;
		Vec4 Column(int m) const;

		float const* Data() const;

		Matrix4x4 operator+(Matrix4x4 const& other) const;
		Matrix4x4 operator-() const;
		Matrix4x4 operator-(Matrix4x4 const& other) const;
		Matrix4x4 operator*(Matrix4x4 const& other) const;

		friend std::ostream& operator<<(std::ostream& os, Matrix4x4 const& mat);

	private:
		std::array<float, 16> _elements {};
};

#endif // !MATRIX4X4_HPP