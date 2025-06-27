#ifndef MATRIX2X2_HPP
#define MATRIX2X2_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <array>
#include <cassert>

#include <Math/Vector2.hpp>

namespace Math {
    class Matrix2x2 {
        public:
            Matrix2x2();
            Matrix2x2(float cc);
            Matrix2x2(float xx, float yy);
            Matrix2x2(float xx, float xy,
                      float yx, float yy);
            Matrix2x2(Vector2 const& xy1, Vector2 const& xy2, bool columnMajor = false);
            Matrix2x2(Matrix2x2 const& m) = default;

            float& operator [] (size_t index);
            float const& operator [] (size_t index) const;
            float& operator () (size_t n, size_t m);
            float const& operator () (size_t n, size_t m) const;
        
            Matrix2x2& operator = (Matrix2x2 const& m);

            friend bool operator == (Matrix2x2 const& lhs, Matrix2x2 const& rhs);
            friend bool operator != (Matrix2x2 const& lhs, Matrix2x2 const& rhs);

            friend std::ostream& operator << (std::ostream& out, Matrix2x2 const& m);

            friend Matrix2x2 operator + (Matrix2x2 const& lhs, Matrix2x2 const& rhs);
            friend Matrix2x2& operator += (Matrix2x2& lhs, Matrix2x2 const& rhs);

            friend Matrix2x2 operator - (Matrix2x2 const& m);
            friend Matrix2x2 operator - (Matrix2x2 const& lhs, Matrix2x2 const& rhs);
            friend Matrix2x2& operator -= (Matrix2x2& lhs, Matrix2x2 const& rhs);

            friend Matrix2x2 operator * (Matrix2x2 const& lhs, float const& rhs);
            friend Matrix2x2 operator * (float const& lhs, Matrix2x2 const& rhs);
            friend Matrix2x2 operator * (Matrix2x2 const& lhs, Matrix2x2 const& rhs);
            friend Matrix2x2& operator *= (Matrix2x2& lhs, float const& rhs);
            friend Matrix2x2& operator *= (Matrix2x2& lhs, Matrix2x2 const& rhs);

            friend Matrix2x2 operator / (Matrix2x2 const& lhs, float const& rhs);
            friend Matrix2x2& operator /= (Matrix2x2& lhs, float const& rhs);

            static Matrix2x2 Identity();

            static Matrix2x2 Transpose(Matrix2x2 const& m);
            static Matrix2x2 Inverse(Matrix2x2 const& m);

            Matrix2x2& Transposed();
            Matrix2x2& Inversed();

            Vector2 Line(size_t n) const;
            Vector2 Column(size_t m) const;

        private:
            std::array<float, 4> _elements {};
    };
}

#endif // MATRIX2X2_HPP