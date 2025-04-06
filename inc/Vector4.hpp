#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <iostream>
#include <iomanip>
#include <cmath>

namespace Math {
	class Vector4 {
		public:
			Vector4();
			Vector4(float c);
			Vector4(float x, float y, float z, float w);
			~Vector4() = default;

		public:
			float X() const { return _x; }
			float Y() const { return _y; }
			float Z() const { return _z; }
			float W() const { return _w; }

			float R() const { return _x; }
			float G() const { return _y; }
			float B() const { return _z; }
			float A() const { return _w; }

			Vector4 operator+(Vector4 const& other) const;
			Vector4 operator-() const;
			Vector4 operator-(Vector4 const& other) const;
			Vector4 operator*(float scalar) const;
			Vector4 operator/(float scalar) const;

			friend std::ostream& operator<<(std::ostream& os, Vector4 const& vec);

			float Magnitude() const;

			static float Dot(Vector4 const& a, Vector4 const& b);

		private:
			float _x = 0.0f;
			float _y = 0.0f;
			float _z = 0.0f;
			float _w = 0.0f;
	};
}

#endif // !VECTOR4_HPP
