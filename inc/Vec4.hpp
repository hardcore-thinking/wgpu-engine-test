#ifndef VEC4_HPP
#define VEC4_HPP

class Vec4 {
	public:
		Vec4();
		Vec4(float x, float y, float z, float w);
		~Vec4() = default;
	
	public:
		float X() const;
		float Y() const;
		float Z() const;
		float W() const;

		float R() const;
		float G() const;
		float B() const;
		float A() const;

		Vec4 operator+(Vec4 const& other) const;
		Vec4 operator-(Vec4 const& other) const;
		Vec4 operator*(float scalar) const;
		Vec4 operator/(float scalar) const;

		static float Dot(Vec4 const& a, Vec4 const& b);

	private:
		float _x = 0.0f;
		float _y = 0.0f;
		float _z = 0.0f;
		float _w = 0.0f;
};

#endif // !VEC4_HPP
