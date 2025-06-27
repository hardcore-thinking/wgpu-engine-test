#include <Math/Vector4.hpp>

namespace Math {
    Vector4::Vector4() {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
        this->w = 0.0f;
    }

    Vector4::Vector4(float c) {
        this->x = c;
        this->y = c;
        this->z = c;
        this->w = c;
    }

    Vector4::Vector4(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Vector4::Vector4(Vector2 const& xy, float z, float w) {
        this->x = xy.x;
        this->y = xy.y;
        this->z = z;
        this->w = w;
    }

    Vector4::Vector4(float x, Vector2 const& yz, float w) {
        this->x = x;
        this->y = yz.x;
        this->z = yz.y;
        this->w = w;
    }

    Vector4::Vector4(float x, float y, Vector2 const& zw) {
        this->x = x;
        this->y = y;
        this->z = zw.x;
        this->w = zw.y;
    }

    Vector4::Vector4(Vector2 const& xy, Vector2 const& zw) {
        this->x = xy.x;
        this->y = xy.y;
        this->z = zw.x;
        this->w = zw.y;
    }

    Vector4::Vector4(Vector3 const& xyz, float w) {
        this->x = xyz.x;
        this->y = xyz.y;
        this->z = xyz.z;
        this->w = w;
    }

    Vector4::Vector4(float x, Vector3 const& yzw) {
        this->x = x;
        this->y = yzw.x;
        this->z = yzw.y;
        this->w = yzw.z;
    }

    Vector4& Vector4::operator = (Vector4 const& m) {
        this->x = m.x;
        this->y = m.y;
        this->z = m.z;
        this->w = m.w;

        return *this;
    }

    bool operator == (Vector4 const& lhs, Vector4 const& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
    }

    bool operator != (Vector4 const& lhs, Vector4 const& rhs) {
        return !(lhs == rhs);
    }

    std::ostream& operator << (std::ostream& out, Vector4 const& m) {
        out << std::setprecision(4) << std::fixed << m.x << " " << m.y << " " << m.z << " " << m.w;

        return out;
    }

    Vector4 operator + (Vector4 const& lhs, Vector4 const& rhs) {
        return Vector4(
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z,
            lhs.w + rhs.w
        );
    }

    Vector4& operator += (Vector4& lhs, Vector4 const& rhs) {
        lhs = lhs + rhs;

        return lhs;
    }

    Vector4 operator - (Vector4 const& m) {
        return Vector4(
            -m.x,
            -m.y,
            -m.z,
            -m.w
        );
    }

    Vector4 operator - (Vector4 const& lhs, Vector4 const& rhs) {
        return lhs + (-rhs);
    }

    Vector4& operator -= (Vector4& lhs, Vector4 const& rhs) {
        lhs = lhs - rhs;

        return lhs;
    }

    Vector4 operator * (Vector4 const& lhs, float const& rhs) {
        return Vector4(
            lhs.x * rhs,
            lhs.y * rhs,
            lhs.z * rhs,
            lhs.w * rhs
        );
    }

    Vector4 operator * (float const& lhs, Vector4 const& rhs) {
        return rhs * lhs;
    }

    Vector4& operator *= (Vector4& lhs, float const& rhs) {
        lhs = lhs * rhs;

        return lhs;
    }

    Vector4 operator / (Vector4 const& lhs, float const& rhs) {
        float inversedRhs = 1 / rhs;

        return lhs * inversedRhs;
    }

    Vector4& operator /= (Vector4& lhs, float const& rhs) {
        float inversedRhs = 1 / rhs;
        lhs = lhs * inversedRhs;

        return lhs;
    }

    float Vector4::Dot(Vector4 const& lhs, Vector4 const& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
    }

    float Vector4::Magnitude(Vector4 const& m) {
        return std::sqrt(Dot(m, m));
    }

    Vector4 Vector4::Normalize(Vector4 const& m) {
        float inversedMagnitude = 1 / Magnitude(m);

        return m * inversedMagnitude;
    }

    Vector4& Vector4::Normalized() {
        float inversedMagnitude = 1 / Magnitude(*this);
        *this *= inversedMagnitude;

        return *this;
    }
}