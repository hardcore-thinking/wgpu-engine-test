#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <iostream>
#include <iomanip>
#include <cmath>

#include <Vector2.hpp>

namespace Math {
    struct Vector3 {
        public:
            Vector3();
            Vector3(float c);
            Vector3(float x, float y, float z);
            Vector3(Vector2 const& xy, float z);
            Vector3(float x, Vector2 const& yz);
            Vector3(Vector3 const& m) = default;
            Vector3(Vector3&& m) = default;

            Vector3& operator = (Vector3 const& m);

            friend bool operator == (Vector3 const& lhs, Vector3 const& rhs);
            friend bool operator != (Vector3 const& lhs, Vector3 const& rhs);

            friend std::ostream& operator << (std::ostream& out, Vector3 const& m);

            friend Vector3 operator + (Vector3 const& lhs, Vector3 const& rhs);
            friend Vector3& operator += (Vector3& lhs, Vector3 const& rhs);

            friend Vector3 operator - (Vector3 const& m);
            friend Vector3 operator - (Vector3 const& lhs, Vector3 const& rhs);
            friend Vector3& operator -= (Vector3& lhs, Vector3 const& rhs);

            friend Vector3 operator * (Vector3 const& lhs, float const& rhs);
            friend Vector3 operator * (float const& lhs, Vector3 const& rhs);
            friend Vector3& operator *= (Vector3& lhs, float const& rhs);

            friend Vector3 operator / (Vector3 const& lhs, float const& rhs);
            friend Vector3& operator /= (Vector3& lhs, float const& rhs);

            static float Dot(Vector3 const& lhs, Vector3 const& rhs);
            static Vector3 Cross(Vector3 const& lhs, Vector3 const& rhs);
            static float Magnitude(Vector3 const& m);
            static Vector3 Normalize(Vector3 const& m);

            [[nodiscard]] Vector3& Normalized();

        public:
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;
    };
}

#endif // VECTOR3_HPP