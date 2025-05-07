#ifndef MATRIX4X4_HPP
#define MATRIX4X4_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <array>
#include <cassert>

#include <Vector4.hpp>

namespace Math {
    class Matrix4x4 {
        public:
            Matrix4x4();
            Matrix4x4(float cc);
            Matrix4x4(float xx, float yy, float zz, float ww);
            Matrix4x4(float xx, float xy, float xz, float xw,
                      float yx, float yy, float yz, float yw,
                      float zx, float zy, float zz, float zw,
                      float wx, float wy, float wz, float ww);
            Matrix4x4(Vector4 const& xyzw1, Vector4 const& xyzw2, Vector4 const& xyzw3, Vector4 const& xyzw4, bool columnMajor = false);
            Matrix4x4(Matrix4x4 const& m) = default;

            float& operator [] (size_t index);
            float const& operator [] (size_t index) const;
            float& operator () (size_t n, size_t m);
            float const& operator () (size_t n, size_t m) const;
        
            Matrix4x4& operator = (Matrix4x4 const& m);

            friend bool operator == (Matrix4x4 const& lhs, Matrix4x4 const& rhs);
            friend bool operator != (Matrix4x4 const& lhs, Matrix4x4 const& rhs);

            friend std::ostream& operator << (std::ostream& out, Matrix4x4 const& m);

            friend Matrix4x4 operator + (Matrix4x4 const& lhs, Matrix4x4 const& rhs);
            friend Matrix4x4& operator += (Matrix4x4& lhs, Matrix4x4 const& rhs);

            friend Matrix4x4 operator - (Matrix4x4 const& m);
            friend Matrix4x4 operator - (Matrix4x4 const& lhs, Matrix4x4 const& rhs);
            friend Matrix4x4& operator -= (Matrix4x4& lhs, Matrix4x4 const& rhs);

            friend Matrix4x4 operator * (Matrix4x4 const& lhs, float const& rhs);
            friend Matrix4x4 operator * (float const& lhs, Matrix4x4 const& rhs);
            friend Matrix4x4 operator * (Matrix4x4 const& lhs, Matrix4x4 const& rhs);
            friend Matrix4x4& operator *= (Matrix4x4& lhs, float const& rhs);
            friend Matrix4x4& operator *= (Matrix4x4& lhs, Matrix4x4 const& rhs);

            friend Matrix4x4 operator / (Matrix4x4 const& lhs, float const& rhs);
            friend Matrix4x4 operator /= (Matrix4x4& lhs, float const& rhs);

            static Matrix4x4 Identity();
 
 			static Matrix4x4 RotateX(float angle);
 			static Matrix4x4 RotateY(float angle);
 			static Matrix4x4 RotateZ(float angle);
 
 			static Matrix4x4 Translate(float tx, float ty, float tz);

            static Matrix4x4 Scale(float s);
 			static Matrix4x4 Scale(float sx, float sy, float sz);
 
 			static Matrix4x4 Perspective(float fov, float ratio, float near, float far);
 			static Matrix4x4 Orthographic(float ratio, float near, float far);
 
 			static Matrix4x4 LookAt(Vector3 const& from, Vector3 const& to, Vector3 const& up);

            static Matrix4x4 Transpose(Matrix4x4 const& m);
            static Matrix4x4 Inverse(Matrix4x4 const& m);

            Matrix4x4& Transposed();
            Matrix4x4& Inversed();

            Vector4 Line(size_t n) const;
            Vector4 Column(size_t m) const;

        private:
            std::array<float, 16> _elements {};
    };
}

#endif // MATRIX4X4_HPP