#ifndef MATRIX4X4_HPP
#define MATRIX4X4_HPP

#include <iostream>
#include <array>

#include <Vec4.hpp>

class Matrix4x4 {
	public:
		Matrix4x4();
		Matrix4x4(float e00, float e01, float e02, float e03,
			float e10, float e11, float e12, float e13,
			float e20, float e21, float e22, float e23,
			float e30, float e31, float e32, float e33);
		~Matrix4x4() = default;

	public:
		static Matrix4x4 Identity();
		static Matrix4x4 Transpose(Matrix4x4 const& mat);

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