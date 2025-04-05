#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <cmath>

class Vector4 {
	public:
		Vector4();
		Vector4(float x, float y, float z, float w);
		~Vector4() = default;
	
	public:
		float X() const;
		float Y() const;
		float Z() const;
		float W() const;

		float R() const;
		float G() const;
		float B() const;
		float A() const;

		Vector4 operator+(Vector4 const& other) const;
		Vector4 operator-() const;
		Vector4 operator-(Vector4 const& other) const;
		Vector4 operator*(float scalar) const;
		Vector4 operator/(float scalar) const;

		float Magnitude() const;

		static float Dot(Vector4 const& a, Vector4 const& b);

	private:
		float _x = 0.0f;
		float _y = 0.0f;
		float _z = 0.0f;
		float _w = 0.0f;
};

#endif // !VECTOR4_HPP
