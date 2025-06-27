#include <Math/Matrix2x2.hpp>

namespace Math {
    Matrix2x2::Matrix2x2() {
        _elements.fill(0.0f);
    }

    Matrix2x2::Matrix2x2(float cc) {
        _elements = {
            cc,   0.0f,
            0.0f,   cc
        };
    }

    Matrix2x2::Matrix2x2(float xx, float yy) {
        _elements = {
              xx, 0.0f,
            0.0f,   yy
        };
    }

    Matrix2x2::Matrix2x2(float xx, float xy,
                         float yx, float yy) {
        _elements = {
            xx, xy,
            yx, yy
        };
    }

    Matrix2x2::Matrix2x2(Vector2 const& xy1, Vector2 const& xy2, bool columnMajor) {
        if (columnMajor) {
            _elements = {
                xy1.x, xy2.x,
                xy1.y, xy2.y
            };
        } else {
            _elements = {
                xy1.x, xy1.y,
                xy2.x, xy2.y
            };
        }
    }

    float& Matrix2x2::operator [] (size_t index) {
        assert("Index must be in the bounds of the matrix"
               && index >= 0 && index < 4);

        return _elements[index];
    }

    float const& Matrix2x2::operator [] (size_t index) const {
        assert("Index must be in the bounds of the matrix"
               && index >= 0 && index < 4);

        return _elements[index];
    }

    float& Matrix2x2::operator () (size_t n, size_t m) {
        assert("Indices must be in the bounds of the matrix"
               && n >= 0 && n < 2
               && m >= 0 && m < 2);

        return _elements[n * 2 + m];
    }

    float const& Matrix2x2::operator () (size_t n, size_t m) const {
        assert("Indices must be in the bounds of the matrix"
               && n >= 0 && n < 2
               && m >= 0 && m < 2);

        return _elements[n * 2 + m];
    }

    Matrix2x2& Matrix2x2::operator = (Matrix2x2 const& m) {
        _elements = m._elements;

        return *this;
    }

    bool operator == (Matrix2x2 const& lhs, Matrix2x2 const& rhs) {
        return (
            lhs.Line(0) == rhs.Line(0) &&
            lhs.Line(1) == rhs.Line(1)
        );
    }

    bool operator != (Matrix2x2 const& lhs, Matrix2x2 const& rhs) {
        return !(lhs == rhs);
    }

    std::ostream& operator << (std::ostream& out, Matrix2x2 const& m) {
        for (int i = 0; i < 4; i++) {
            out << std::setprecision(4) << std::fixed << ((i != 0 && i % 2 == 0) ? "\n" : "") << m[i] << " ";
        }

        return out;
    }

    Matrix2x2 operator + (Matrix2x2 const& lhs, Matrix2x2 const& rhs) {
        return Matrix2x2(
            lhs(0, 0) + rhs(0, 0), lhs(0, 1) + rhs(0, 1),
            lhs(1, 0) + rhs(1, 0), lhs(1, 1) + rhs(1, 1)
        );
    }

    Matrix2x2& operator += (Matrix2x2& lhs, Matrix2x2 const& rhs) {
        lhs = lhs + rhs;

        return lhs;
    }

    Matrix2x2 operator - (Matrix2x2 const& m) {
        return Matrix2x2(
            -m(0, 0), -m(0, 1),
            -m(1, 0), -m(1, 1)
        );
    }

    Matrix2x2 operator - (Matrix2x2 const& lhs, Matrix2x2 const& rhs) {
        return lhs + (-rhs);
    }

    Matrix2x2& operator -= (Matrix2x2& lhs, Matrix2x2 const& rhs) {
        lhs = lhs - rhs;

        return lhs;
    }

    Matrix2x2 operator * (Matrix2x2 const& lhs, float const& rhs) {
        return Matrix2x2(
            rhs * lhs(0, 0), rhs * lhs(0, 1),
            rhs * lhs(1, 0), rhs * lhs(1, 1)
        );
    }

    Matrix2x2 operator * (float const& lhs, Matrix2x2 const& rhs) {
        return rhs * lhs;
    }

    Matrix2x2 operator * (Matrix2x2 const& lhs, Matrix2x2 const& rhs) {
        return Matrix2x2(
            Vector2::Dot(lhs.Line(0), rhs.Column(0)),
            Vector2::Dot(lhs.Line(0), rhs.Column(1)),

            Vector2::Dot(lhs.Line(1), rhs.Column(0)),
            Vector2::Dot(lhs.Line(1), rhs.Column(1))
        );
    }

    Matrix2x2& operator *= (Matrix2x2& lhs, float const& rhs) {
        lhs = lhs * rhs;

        return lhs;
    }

    Matrix2x2& operator *= (Matrix2x2& lhs, Matrix2x2 const& rhs) {
        lhs = lhs * rhs;

        return lhs;
    }

    Matrix2x2 operator / (Matrix2x2 const& lhs, float const& rhs) {
        float inversedRhs = 1 / rhs;
        return Matrix2x2(
            lhs(0, 0) * inversedRhs, lhs(0, 1) * inversedRhs,
            lhs(1, 0) * inversedRhs, lhs(1, 1) * inversedRhs
        );
    }

    Matrix2x2& operator /= (Matrix2x2& lhs, float const& rhs) {
        lhs = lhs / rhs;

        return lhs;
    }

    float Matrix2x2::Determinant(Matrix2x2 const& m) {
        return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
    }

    Matrix2x2 Matrix2x2::Identity() {
        return Matrix2x2(1.0f);
    }

    Matrix2x2 Matrix2x2::Transpose(Matrix2x2 const& m) {
        return Matrix2x2(
            m(0, 0), m(1, 0),
            m(0, 1), m(1, 1)
        );
    }

    Matrix2x2 Matrix2x2::Inverse(Matrix2x2 const& m) {
        float det = Determinant(m);
        assert("Matrix is not invertible"
               && det != 0.0f);

        return Matrix2x2(
             m(1, 1) / det, -m(0, 1) / det,
            -m(1, 0) / det,  m(0, 0) / det
        );
    }

    Matrix2x2& Matrix2x2::Transposed() {
        *this = Transpose(*this);

        return *this;
    }

    Matrix2x2& Matrix2x2::Inversed() {
        _elements = Inverse(*this)._elements;

        return *this;
    }

    Vector2 Matrix2x2::Line(size_t n) const {
        assert("Indices must be in the bounds of the matrix"
               && n >= 0 && n < 2);
        return Vector2(
            (*this)(n, 0),
            (*this)(n, 1)
        );
    }

    Vector2 Matrix2x2::Column(size_t m) const {
        assert("Indices must be in the bounds of the matrix"
               && m >= 0 && m < 2);
        return Vector2(
            (*this)(0, m),
            (*this)(1, m)
        );
    }
}