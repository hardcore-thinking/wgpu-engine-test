#include <Math/Matrix3x3.hpp>

namespace Math
{
    Matrix3x3::Matrix3x3()
    {
        _elements.fill(0.0f);
    }

    Matrix3x3::Matrix3x3(float cc)
    {
        _elements = {
            cc, 0.0f, 0.0f,
            0.0f, cc, 0.0f,
            0.0f, 0.0f, cc};
    }

    Matrix3x3::Matrix3x3(float xx, float yy, float zz)
    {
        _elements = {
            xx, 0.0f, 0.0f,
            0.0f, yy, 0.0f,
            0.0f, 0.0f, zz};
    }

    Matrix3x3::Matrix3x3(float xx, float xy, float xz,
                         float yx, float yy, float yz,
                         float zx, float zy, float zz)
    {
        _elements = {
            xx, xy, xz,
            yx, yy, yz,
            zx, zy, zz};
    }

    Matrix3x3::Matrix3x3(Vector3 const &xyz1, Vector3 const &xyz2, Vector3 const &xyz3, bool columnMajor)
    {
        if (columnMajor)
        {
            _elements = {
                xyz1.x, xyz2.x, xyz3.x,
                xyz1.y, xyz2.y, xyz3.y,
                xyz1.z, xyz2.z, xyz3.z};
        }
        else
        {
            _elements = {
                xyz1.x, xyz1.y, xyz1.z,
                xyz2.x, xyz2.y, xyz2.z,
                xyz3.x, xyz3.y, xyz3.z};
        }
    }

    float &Matrix3x3::operator[](int index)
    {
        assert("Index must be in the bounds of the matrix" && index >= 0 && index < 9);

        return _elements[index];
    }

    float const &Matrix3x3::operator[](int index) const
    {
        assert("Index must be in the bounds of the matrix" && index >= 0 && index < 9);

        return _elements[index];
    }

    float &Matrix3x3::operator()(int n, int m)
    {
        assert("Indices must be in the bounds of the matrix" && n >= 0 && n < 3 && m >= 0 && m < 3);

        return _elements[n * 3 + m];
    }

    float const &Matrix3x3::operator()(int n, int m) const
    {
        assert("Indices must be in the bounds of the matrix" && n >= 0 && n < 3 && m >= 0 && m < 3);

        return _elements[n * 3 + m];
    }

    Matrix3x3 &Matrix3x3::operator=(Matrix3x3 const &m)
    {
        _elements = m._elements;

        return *this;
    }

    bool operator==(Matrix3x3 const &lhs, Matrix3x3 const &rhs)
    {
        return (
            lhs.Line(0) == rhs.Line(0) &&
            lhs.Line(1) == rhs.Line(1) &&
            lhs.Line(2) == rhs.Line(2));
    }

    bool operator!=(Matrix3x3 const &lhs, Matrix3x3 const &rhs)
    {
        return !(lhs == rhs);
    }

    std::ostream &operator<<(std::ostream &out, Matrix3x3 const &m)
    {
        for (int i = 0; i < 9; i++)
        {
            out << std::setprecision(4) << std::fixed << ((i != 0 && i % 3 == 0) ? "\n" : "") << m[i] << " ";
        }

        return out;
    }

    Matrix3x3 operator+(Matrix3x3 const &lhs, Matrix3x3 const &rhs)
    {
        return Matrix3x3(
            lhs(0, 0) + rhs(0, 0), lhs(0, 1) + rhs(0, 1), lhs(0, 2) + rhs(0, 2),
            lhs(1, 0) + rhs(1, 0), lhs(1, 1) + rhs(1, 1), lhs(1, 2) + rhs(1, 2),
            lhs(2, 0) + rhs(2, 0), lhs(2, 1) + rhs(2, 1), lhs(2, 2) + rhs(2, 2));
    }

    Matrix3x3 &operator+=(Matrix3x3 &lhs, Matrix3x3 const &rhs)
    {
        lhs = lhs + rhs;

        return lhs;
    }

    Matrix3x3 operator-(Matrix3x3 const &m)
    {
        return Matrix3x3(
            -m(0, 0), -m(0, 1), -m(0, 2),
            -m(1, 0), -m(1, 1), -m(1, 2),
            -m(2, 0), -m(2, 1), -m(2, 2));
    }

    Matrix3x3 operator-(Matrix3x3 const &lhs, Matrix3x3 const &rhs)
    {
        return lhs + (-rhs);
    }

    Matrix3x3 &operator-=(Matrix3x3 &lhs, Matrix3x3 const &rhs)
    {
        lhs = lhs - rhs;

        return lhs;
    }

    Matrix3x3 operator*(Matrix3x3 const &lhs, float const &rhs)
    {
        return Matrix3x3(
            rhs * lhs(0, 0), rhs * lhs(0, 1), rhs * lhs(0, 2),
            rhs * lhs(1, 0), rhs * lhs(1, 1), rhs * lhs(1, 2),
            rhs * lhs(2, 0), rhs * lhs(2, 1), rhs * lhs(2, 2));
    }

    Matrix3x3 operator*(float const &lhs, Matrix3x3 const &rhs)
    {
        return rhs * lhs;
    }

    Matrix3x3 operator*(Matrix3x3 const &lhs, Matrix3x3 const &rhs)
    {
        return Matrix3x3(
            Vector3::Dot(lhs.Line(0), rhs.Column(0)),
            Vector3::Dot(lhs.Line(0), rhs.Column(1)),
            Vector3::Dot(lhs.Line(0), rhs.Column(2)),

            Vector3::Dot(lhs.Line(1), rhs.Column(0)),
            Vector3::Dot(lhs.Line(1), rhs.Column(1)),
            Vector3::Dot(lhs.Line(1), rhs.Column(2)),

            Vector3::Dot(lhs.Line(2), rhs.Column(0)),
            Vector3::Dot(lhs.Line(2), rhs.Column(1)),
            Vector3::Dot(lhs.Line(2), rhs.Column(2)));
    }

    Matrix3x3 &operator*=(Matrix3x3 &lhs, float const &rhs)
    {
        lhs = lhs * rhs;

        return lhs;
    }

    Matrix3x3 &operator*=(Matrix3x3 &lhs, Matrix3x3 const &rhs)
    {
        lhs = lhs * rhs;

        return lhs;
    }

    Matrix3x3 operator/(Matrix3x3 const &lhs, float const &rhs)
    {
        float inversedRhs = 1 / rhs;
        return Matrix3x3(
            lhs(0, 0) * inversedRhs, lhs(0, 1) * inversedRhs, lhs(0, 2) * inversedRhs,
            lhs(1, 0) * inversedRhs, lhs(1, 1) * inversedRhs, lhs(1, 2) * inversedRhs,
            lhs(2, 0) * inversedRhs, lhs(2, 1) * inversedRhs, lhs(2, 2) * inversedRhs);
    }

    Matrix3x3 &operator/=(Matrix3x3 &lhs, float const &rhs)
    {
        lhs = lhs / rhs;

        return lhs;
    }

    float Matrix3x3::Determinant(Matrix3x3 const &m)
    {
        return m(0, 0) * Matrix2x2::Determinant(m.Submatrix(0, 0)) -
               m(0, 1) * Matrix2x2::Determinant(m.Submatrix(0, 1)) +
               m(0, 2) * Matrix2x2::Determinant(m.Submatrix(0, 2));
    }

    Matrix3x3 Matrix3x3::Identity()
    {
        return Matrix3x3(1.0f);
    }

    Matrix3x3 Matrix3x3::RotateX(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return Matrix3x3(
            1.0f, 0.0f, 0.0f,
            0.0f, c, -s,
            0.0f, s, c);
    }

    Matrix3x3 Matrix3x3::RotateY(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return Matrix3x3(
            c, 0.0f, s,
            0.0f, 1.0f, 0.0f,
            -s, 0.0f, c);
    }

    Matrix3x3 Matrix3x3::RotateZ(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return Matrix3x3(
            c, -s, 0.0f,
            s, c, 0.0f,
            0.0f, 0.0f, 1.0f);
    }

    Matrix3x3 Matrix3x3::Translate(float tx, float ty)
    {
        return Matrix3x3(
            1.0f, 0.0f, tx,
            0.0f, 1.0f, ty,
            0.0f, 0.0f, 1.0f);
    }

    Matrix3x3 Matrix3x3::Scale(float s)
    {
        return Matrix3x3(
            s, 0.0f, 0.0f,
            0.0f, s, 0.0f,
            0.0f, 0.0f, 1.0f);
    }

    Matrix3x3 Matrix3x3::Scale(float sx, float sy)
    {
        return Matrix3x3(
            sx, 0.0f, 0.0f,
            0.0f, sy, 0.0f,
            0.0f, 0.0f, 1.0f);
    }

    Matrix3x3 Matrix3x3::Transpose(Matrix3x3 const &m)
    {
        return Matrix3x3(
            m(0, 0), m(1, 0), m(2, 0),
            m(0, 1), m(1, 1), m(2, 1),
            m(0, 2), m(1, 2), m(2, 2));
    }

    Matrix3x3 Matrix3x3::Inverse(Matrix3x3 const &m)
    {
        float det = Determinant(m);
        assert("Matrix is not invertible" && det != 0.0f);

        return Transpose(m.CofactorMatrix()) / det;
    }

    Matrix3x3 &Matrix3x3::Transposed()
    {
        *this = Transpose(*this);

        return *this;
    }

    Matrix3x3 &Matrix3x3::Inversed()
    {
        _elements = Inverse(*this)._elements;

        return *this;
    }

    Vector3 Matrix3x3::Line(int n) const
    {
        assert("Indices must be in the bounds of the matrix" && n >= 0 && n < 3);
        return Vector3(
            (*this)(n, 0),
            (*this)(n, 1),
            (*this)(n, 2));
    }

    Vector3 Matrix3x3::Column(int m) const
    {
        assert("Indices must be in the bounds of the matrix" && m >= 0 && m < 3);
        return Vector3(
            (*this)(0, m),
            (*this)(1, m),
            (*this)(2, m));
    }

    Matrix2x2 Matrix3x3::Submatrix(int i, int j) const
    {
        assert("Indices must be in the bounds of the matrix" && i >= 0 && i < 3 && j >= 0 && j < 3);

        Matrix2x2 sub;
        int index = 0;

        for (int row = 0; row < 3; ++row)
        {
            if (row == i)
                continue;
            for (int col = 0; col < 3; ++col)
            {
                if (col == j)
                    continue;
                sub[index++] = (*this)(row, col);
            }
        }

        return sub;
    }

    float Matrix3x3::Cofactor(int i, int j) const
    {
        assert("Indices must be in the bounds of the matrix" && i >= 0 && i < 3 && j >= 0 && j < 3);

        return ((i + j) % 2 == 0 ? 1.0f : -1.0f) * Matrix2x2::Determinant(Submatrix(i, j));
    }

    Matrix3x3 Matrix3x3::CofactorMatrix() const
    {
        Matrix3x3 cofactor;

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                cofactor(i, j) = Cofactor(i, j);
            }
        }

        return cofactor;
    }
}