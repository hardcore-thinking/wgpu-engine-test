#include <Math/Matrix4x4.hpp>

namespace Math {
    Matrix4x4::Matrix4x4() {
        _elements.fill(0.0f);
    }

    Matrix4x4::Matrix4x4(float cc) {
        _elements = {
              cc, 0.0f, 0.0f, 0.0f,
            0.0f,   cc, 0.0f, 0.0f,
            0.0f, 0.0f,   cc, 0.0f,
            0.0f, 0.0f, 0.0f,   cc
        };
    }

    Matrix4x4::Matrix4x4(float xx, float yy, float zz, float ww) {
        _elements = {
              xx, 0.0f, 0.0f, 0.0f,
            0.0f,   yy, 0.0f, 0.0f,
            0.0f, 0.0f,   zz, 0.0f,
            0.0f, 0.0f, 0.0f,   ww
        };
    }

    Matrix4x4::Matrix4x4(float xx, float xy, float xz, float xw,
                         float yx, float yy, float yz, float yw,
                         float zx, float zy, float zz, float zw,
                         float wx, float wy, float wz, float ww) {
        _elements = {
            xx, xy, xz, xw,
            yx, yy, yz, yw,
            zx, zy, zz, zw,
            wx, wy, wz, ww
        };
    }

    Matrix4x4::Matrix4x4(Vector4 const& xyzw1, Vector4 const& xyzw2, Vector4 const& xyzw3, Vector4 const& xyzw4, bool columnMajor) {
        if (columnMajor) {
            _elements = {
                xyzw1.x, xyzw2.x, xyzw3.x, xyzw4.x,
                xyzw1.y, xyzw2.y, xyzw3.y, xyzw4.y,
                xyzw1.z, xyzw2.z, xyzw3.z, xyzw4.z,
                xyzw1.w, xyzw2.w, xyzw3.w, xyzw4.w
            };
        } else {
            _elements = {
                xyzw1.x, xyzw1.y, xyzw1.z, xyzw1.w,
                xyzw2.x, xyzw2.y, xyzw2.z, xyzw2.w,
                xyzw3.x, xyzw3.y, xyzw3.z, xyzw3.w,
                xyzw4.x, xyzw4.y, xyzw4.z, xyzw4.w
            };
        }
    }

    float& Matrix4x4::operator [] (size_t index) {
        assert("Index must be in the bounds of the matrix"
            && index >= 0 && index < 16);

        return _elements[index];
    }

    float const& Matrix4x4::operator [] (size_t index) const {
        assert("Index must be in the bounds of the matrix"
            && index >= 0 && index < 16);

        return _elements[index];
    }

    float& Matrix4x4::operator () (size_t n, size_t m) {
        assert("Indices must be in the bounds of the matrix"
            && n >= 0 && n < 4
            && m >= 0 && m < 4);
    
        return _elements[n * 4 + m];
    }

    float const& Matrix4x4::operator () (size_t n, size_t m) const {
        assert("Indices must be in the bounds of the matrix"
            && n >= 0 && n < 4
            && m >= 0 && m < 4);
    
        return _elements[n * 4 + m];
    }

    Matrix4x4& Matrix4x4::operator = (Matrix4x4 const& m) {
        this->_elements = m._elements;

        return *this;
    }

    bool operator == (Matrix4x4 const& lhs, Matrix4x4 const& rhs) {
        return (
            lhs.Line(0) == rhs.Line(0) &&
            lhs.Line(1) == rhs.Line(1) &&
            lhs.Line(2) == rhs.Line(2) &&
            lhs.Line(3) == rhs.Line(3)
        );
    }

    bool operator != (Matrix4x4 const& lhs, Matrix4x4 const& rhs) {
        return !(lhs == rhs);
    }

    std::ostream& operator << (std::ostream& out, Matrix4x4 const& m) {
        for (int i = 0; i < 16; i++) {
            out << std::setprecision(2) << std::fixed << ((i != 0 && i % 4 == 0) ? "\n" : "") << m[i] << " ";
        }

        return out;
    }

    Matrix4x4 operator + (Matrix4x4 const& lhs, Matrix4x4 const& rhs) {
        return Matrix4x4(
            lhs(0, 0) + rhs(0, 0), lhs(0, 1) + rhs(0, 1), lhs(0, 2) + rhs(0, 2), lhs(0, 3) + rhs(0, 3),
            lhs(1, 0) + rhs(1, 0), lhs(1, 1) + rhs(1, 1), lhs(1, 2) + rhs(1, 2), lhs(1, 3) + rhs(1, 3),
            lhs(2, 0) + rhs(2, 0), lhs(2, 1) + rhs(2, 1), lhs(2, 2) + rhs(2, 2), lhs(2, 3) + rhs(2, 3),
            lhs(3, 0) + rhs(3, 0), lhs(3, 1) + rhs(3, 1), lhs(3, 2) + rhs(3, 2), lhs(3, 3) + rhs(3, 3)
        );
    }

    Matrix4x4& operator += (Matrix4x4& lhs, Matrix4x4 const& rhs) {
        lhs = lhs + rhs;

        return lhs;
    }

    Matrix4x4 operator - (Matrix4x4 const& m) {
        return Matrix4x4(
            -m(0, 0), -m(0, 1), -m(0, 2), -m(0, 3),
            -m(1, 0), -m(1, 1), -m(1, 2), -m(1, 3),
            -m(2, 0), -m(2, 1), -m(2, 2), -m(2, 3),
            -m(3, 0), -m(3, 1), -m(3, 2), -m(3, 3)
        );
    }

    Matrix4x4 operator - (Matrix4x4 const& lhs, Matrix4x4 const& rhs) {
        return lhs + (-rhs);
    }

    Matrix4x4& operator -= (Matrix4x4& lhs, Matrix4x4 const& rhs) {
        lhs = lhs - rhs;

        return lhs;
    }

    Matrix4x4 operator * (Matrix4x4 const& lhs, float const& rhs) {
        return Matrix4x4(
            rhs * lhs(0, 0), rhs * lhs(0, 1), rhs * lhs(0, 2), rhs * lhs(0, 3),
            rhs * lhs(1, 0), rhs * lhs(1, 1), rhs * lhs(1, 2), rhs * lhs(1, 3),
            rhs * lhs(2, 0), rhs * lhs(2, 1), rhs * lhs(2, 2), rhs * lhs(2, 3),
            rhs * lhs(3, 0), rhs * lhs(3, 1), rhs * lhs(3, 2), rhs * lhs(3, 3)
        );
    }

    Matrix4x4 operator * (float const& lhs, Matrix4x4 const& rhs) {
        return rhs * lhs;
    }

    Matrix4x4 operator * (Matrix4x4 const& lhs, Matrix4x4 const& rhs) {
        return Matrix4x4(
            Vector4::Dot(lhs.Line(0), rhs.Column(0)),
            Vector4::Dot(lhs.Line(0), rhs.Column(1)),
            Vector4::Dot(lhs.Line(0), rhs.Column(2)),
            Vector4::Dot(lhs.Line(0), rhs.Column(3)),

            Vector4::Dot(lhs.Line(1), rhs.Column(0)),
            Vector4::Dot(lhs.Line(1), rhs.Column(1)),
            Vector4::Dot(lhs.Line(1), rhs.Column(2)),
            Vector4::Dot(lhs.Line(1), rhs.Column(3)),

            Vector4::Dot(lhs.Line(2), rhs.Column(0)),
            Vector4::Dot(lhs.Line(2), rhs.Column(1)),
            Vector4::Dot(lhs.Line(2), rhs.Column(2)),
            Vector4::Dot(lhs.Line(2), rhs.Column(3)),

            Vector4::Dot(lhs.Line(3), rhs.Column(0)),
            Vector4::Dot(lhs.Line(3), rhs.Column(1)),
            Vector4::Dot(lhs.Line(3), rhs.Column(2)),
            Vector4::Dot(lhs.Line(3), rhs.Column(3))
        );
    }

    Matrix4x4& operator *= (Matrix4x4& lhs, float const& rhs) {
        lhs = lhs * rhs;
    
        return lhs;
    }

    Matrix4x4& operator *= (Matrix4x4& lhs, Matrix4x4 const& rhs) {
        lhs = lhs * rhs;

        return lhs;
    }

    Matrix4x4 operator / (Matrix4x4 const& lhs, float const& rhs) {
        float inversedRhs = 1 / rhs;
        return Matrix4x4(
            lhs(0, 0) * inversedRhs, lhs(0, 1) * inversedRhs, lhs(0, 2) * inversedRhs, lhs(0, 3) * inversedRhs,
            lhs(1, 0) * inversedRhs, lhs(1, 1) * inversedRhs, lhs(1, 2) * inversedRhs, lhs(1, 3) * inversedRhs,
            lhs(2, 0) * inversedRhs, lhs(2, 1) * inversedRhs, lhs(2, 2) * inversedRhs, lhs(2, 3) * inversedRhs,
            lhs(3, 0) * inversedRhs, lhs(3, 1) * inversedRhs, lhs(3, 2) * inversedRhs, lhs(3, 3) * inversedRhs
        );
    }

    Matrix4x4 operator /= (Matrix4x4& lhs, float const& rhs) {
        lhs = lhs / rhs;

        return lhs;
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
 			  right.x,   right.y,   right.z, translation.x,
 			     up.x,      up.y,      up.z, translation.y,
 			forward.x, forward.y, forward.z, translation.z,
 			     0.0f,      0.0f,      0.0f,          1.0f
 		);
 	}

    Matrix4x4 Matrix4x4::Transpose(Matrix4x4 const& m) {
        return Matrix4x4(
            m(0, 0), m(1, 0), m(2, 0), m(3, 0),
            m(0, 1), m(1, 1), m(2, 1), m(3, 1),
            m(0, 2), m(1, 2), m(2, 2), m(3, 2),
            m(0, 3), m(1, 3), m(2, 3), m(3, 3)
        );
    }

    Matrix4x4& Matrix4x4::Transposed() {
        _elements = {
            _elements[0], _elements[4],  _elements[8], _elements[12],
            _elements[1], _elements[5],  _elements[9], _elements[13],
            _elements[2], _elements[6], _elements[10], _elements[14],
            _elements[3], _elements[7], _elements[11], _elements[15]
        };

        return *this;
    }

    Vector4 Matrix4x4::Line(size_t n) const {
        assert("Indices must be in the bounds of the matrix"
            && n >= 0 && n < 4);
        return Vector4(
            (*this)(n, 0),
            (*this)(n, 1),
            (*this)(n, 2),
            (*this)(n, 3)
        );
    }

    Vector4 Matrix4x4::Column(size_t m) const {
        assert("Indices must be in the bounds of the matrix"
            && m >= 0 && m < 4);
        return Vector4(
            (*this)(0, m),
            (*this)(1, m),
            (*this)(2, m),
            (*this)(3, m)
        );
    }
}