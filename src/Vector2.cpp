#include <Vector2.hpp>

Vector2::Vector2() {
    this->x = 0.0f;
    this->y = 0.0f;
}

Vector2::Vector2(float c) {
    this->x = c;
    this->y = c;
}

Vector2::Vector2(float x, float y) {
    this->x = x;
    this->y = y;
}

Vector2& Vector2::operator = (Vector2 const& m) {
    this->x = m.x;
    this->y = m.y;

    return *this;
}

bool operator == (Vector2 const& lhs, Vector2 const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator != (Vector2 const& lhs, Vector2 const& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator << (std::ostream& out, Vector2 const& m) {
    out << std::setprecision(3) << "[" << std::fixed << m.x << ", " << m.y << "]";

    return out;
}

Vector2 operator + (Vector2 const& lhs, Vector2 const& rhs) {
    return Vector2(
        lhs.x + rhs.x,
        lhs.y + rhs.y
    );
}

Vector2& operator += (Vector2& lhs, Vector2 const& rhs) {
    lhs = lhs + rhs;

    return lhs;
}

Vector2 operator - (Vector2 const& m) {
    return Vector2(
        -m.x,
        -m.y
    );
}

Vector2 operator - (Vector2 const& lhs, Vector2 const& rhs) {
    return lhs + (-rhs);
}

Vector2& operator -= (Vector2& lhs, Vector2 const& rhs) {
    lhs = lhs - rhs;

    return lhs;
}

Vector2 operator * (Vector2 const& lhs, float const& rhs) {
    return Vector2(
        lhs.x * rhs,
        lhs.y * rhs
    );
}

Vector2 operator * (float const& lhs, Vector2 const& rhs) {
    return rhs * lhs;
}

Vector2& operator *= (Vector2& lhs, float const& rhs) {
    lhs = lhs * rhs;

    return lhs;
}

Vector2 operator / (Vector2 const& lhs, float const& rhs) {
    float inversedRhs = 1 / rhs;
    return lhs * inversedRhs;
}

Vector2& operator /= (Vector2& lhs, float const& rhs) {
    lhs = lhs / rhs;

    return lhs;
}

float Vector2::Dot(Vector2 const& lhs, Vector2 const& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

float Vector2::Magnitude(Vector2 const& m) {
    return std::sqrt(Dot(m, m));
}

Vector2 Vector2::Normalize(Vector2 const& m) {
    float inversedMagnitude = 1 / Magnitude(m);

    return m * inversedMagnitude;
}

Vector2& Vector2::Normalized() {
    float inversedMagnitude = 1 / Magnitude(*this);
    *this *= inversedMagnitude;

    return *this ;
}
