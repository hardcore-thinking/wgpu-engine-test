#include <Math/Vector3.hpp>

namespace Math {
    Vector3::Vector3() {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
    }

    Vector3::Vector3(float c) {
        this->x = c;
        this->y = c;
        this->z = c;
    }

    Vector3::Vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3::Vector3(Vector2 const& xy, float z) {
        this->x = xy.x;
        this->y = xy.y;
        this->z = z;
    }

    Vector3::Vector3(float x, Vector2 const& yz) {
        this->x = x;
        this->y = yz.x;
        this->z = yz.y;
    }

    Vector3& Vector3::operator = (Vector3 const& m) {
        this->x = m.x;
        this->y = m.y;
        this->z = m.z;

        return *this;
    }

    bool operator == (Vector3 const& lhs, Vector3 const& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }

    bool operator != (Vector3 const& lhs, Vector3 const& rhs) {
        return !(lhs == rhs);
    }

    std::ostream& operator << (std::ostream& out, Vector3 const& m) {
        out << std::setprecision(2) << std::fixed << m.x << " " << m.y << " " << m.z;

        return out;
    }

    Vector3 operator + (Vector3 const& lhs, Vector3 const& rhs) {
        return Vector3(
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z
        );
    }

    Vector3& operator += (Vector3& lhs, Vector3 const& rhs) {
        lhs = lhs + rhs;

        return lhs;
    }

    Vector3 operator - (Vector3 const& m) {
        return Vector3(
            -m.x,
            -m.y,
            -m.z
        );
    }

    Vector3 operator - (Vector3 const& lhs, Vector3 const& rhs) {
        return lhs + (-rhs);
    }

    Vector3& operator -= (Vector3& lhs, Vector3 const& rhs) {
        lhs = lhs - rhs;

        return lhs;
    }

    Vector3 operator * (Vector3 const& lhs, float const& rhs) {
        return Vector3(
            lhs.x * rhs,
            lhs.y * rhs,
            lhs.z * rhs
        );
    }

    Vector3 operator * (float const& lhs, Vector3 const& rhs) {
        return rhs * lhs;
    }

    Vector3& operator *= (Vector3& lhs, float const& rhs) {
        lhs = lhs * rhs;

        return lhs;
    }

    Vector3 operator / (Vector3 const& lhs, float const& rhs) {
        float inversedRhs = 1 / rhs;
        return lhs * inversedRhs;
    }

    Vector3& operator /= (Vector3& lhs, float const& rhs) {
        lhs = lhs / rhs;

        return lhs;
    }

    float Vector3::Dot(Vector3 const& lhs, Vector3 const& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }

    float Vector3::Magnitude(Vector3 const& m) {
        return std::sqrt(Dot(m, m));
    }

    Vector3 Vector3::Cross(Vector3 const& lhs, Vector3 const& rhs) {
        return Vector3(
            lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x
        );
    }

    Vector3 Vector3::Normalize(Vector3 const& m) {
        float inversedMagnitude = 1 / Magnitude(m);

        return m * inversedMagnitude;
    }

    Vector3& Vector3::Normalized() {
        float inversedMagnitude = 1 / Magnitude(*this);
        *this *= inversedMagnitude;

        return *this;
    }
}