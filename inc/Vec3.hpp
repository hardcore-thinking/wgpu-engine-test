#ifndef VEC3_HPP
#define VEC3_HPP

class Vec3 {
public:
	Vec3();
	Vec3(float x, float y, float z);
	~Vec3() = default;

public:
	float X() const;
	float Y() const;
	float Z() const;

	float R() const;
	float G() const;
	float B() const;

	Vec3 operator+(Vec3 const& other) const;
	Vec3 operator-(Vec3 const& other) const;
	Vec3 operator*(float scalar) const;
	Vec3 operator/(float scalar) const;

	static float Dot(Vec3 const& a, Vec3 const& b);

private:
	float _x = 0.0f;
	float _y = 0.0f;
	float _z = 0.0f;
};

#endif // !VEC3_HPP
