#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <iostream>
#include <iomanip>
#include <cmath>

namespace Math {
	class Vector3 {
		public:
			Vector3();
			Vector3(float c);
			Vector3(float x, float y, float z);
			~Vector3() = default;

		public:
			float X() const { return _x; }
			float Y() const { return _y; }
			float Z() const { return _z; }

			float R() const { return _x; }
			float G() const { return _y; }
			float B() const { return _z; }

			Vector3 operator+(Vector3 const& other) const;
			Vector3 operator-(Vector3 const& other) const;
			Vector3 operator-() const;
			Vector3 operator*(float scalar) const;
			Vector3 operator/(float scalar) const;

			friend std::ostream& operator<<(std::ostream& os, Vector3 const& vec);

			float Magnitude() const;

			static Vector3 Normalize(Vector3 const& vec);
			static float Dot(Vector3 const& a, Vector3 const& b);
			static Vector3 Cross(Vector3 const& a, Vector3 const& b);

		private:
			float _x = 0.0f;
			float _y = 0.0f;
			float _z = 0.0f;
		};
}

#endif // !VECTOR3_HPP
