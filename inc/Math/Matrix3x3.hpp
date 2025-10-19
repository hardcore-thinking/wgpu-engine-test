#ifndef MATRIX3X3_HPP
#define MATRIX3X3_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <array>
#include <cassert>

#include <Math/Matrix2x2.hpp>
#include <Math/Vector3.hpp>

namespace Math
{
    class Matrix3x3
    {
    public:
        Matrix3x3();
        Matrix3x3(float cc);
        Matrix3x3(float xx, float yy, float zz);
        Matrix3x3(float xx, float xy, float xz,
                  float yx, float yy, float yz,
                  float zx, float zy, float zz);
        Matrix3x3(Vector3 const &xyz1, Vector3 const &xyz2, Vector3 const &xyz3, bool columnMajor = false);
        Matrix3x3(Matrix3x3 const &m) = default;

        float &operator[](int index);
        float const &operator[](int index) const;
        float &operator()(int n, int m);
        float const &operator()(int n, int m) const;

        Matrix3x3 &operator=(Matrix3x3 const &m);

        friend bool operator==(Matrix3x3 const &lhs, Matrix3x3 const &rhs);
        friend bool operator!=(Matrix3x3 const &lhs, Matrix3x3 const &rhs);

        friend std::ostream &operator<<(std::ostream &out, Matrix3x3 const &m);

        friend Matrix3x3 operator+(Matrix3x3 const &lhs, Matrix3x3 const &rhs);
        friend Matrix3x3 &operator+=(Matrix3x3 &lhs, Matrix3x3 const &rhs);

        friend Matrix3x3 operator-(Matrix3x3 const &m);
        friend Matrix3x3 operator-(Matrix3x3 const &lhs, Matrix3x3 const &rhs);
        friend Matrix3x3 &operator-=(Matrix3x3 &lhs, Matrix3x3 const &rhs);

        friend Matrix3x3 operator*(Matrix3x3 const &lhs, float const &rhs);
        friend Matrix3x3 operator*(float const &lhs, Matrix3x3 const &rhs);
        friend Matrix3x3 operator*(Matrix3x3 const &lhs, Matrix3x3 const &rhs);
        friend Matrix3x3 &operator*=(Matrix3x3 &lhs, float const &rhs);
        friend Matrix3x3 &operator*=(Matrix3x3 &lhs, Matrix3x3 const &rhs);

        friend Matrix3x3 operator/(Matrix3x3 const &lhs, float const &rhs);
        friend Matrix3x3 &operator/=(Matrix3x3 &lhs, float const &rhs);

        static float Determinant(Matrix3x3 const &m);
        static Matrix3x3 Identity();

        static Matrix3x3 RotateX(float angle);
        static Matrix3x3 RotateY(float angle);
        static Matrix3x3 RotateZ(float angle);

        static Matrix3x3 Translate(float tx, float ty);

        static Matrix3x3 Scale(float s);
        static Matrix3x3 Scale(float sx, float sy);

        static Matrix3x3 Transpose(Matrix3x3 const &m);
        static Matrix3x3 Inverse(Matrix3x3 const &m);

        Matrix3x3 &Transposed();
        Matrix3x3 &Inversed();

        Vector3 Line(int n) const;
        Vector3 Column(int m) const;

        Matrix2x2 Submatrix(int i, int j) const;
        float Cofactor(int i, int j) const;
        Matrix3x3 CofactorMatrix() const;

    private:
        std::array<float, 9> _elements{};
    };
}

#endif // MATRIX3X3_HPP