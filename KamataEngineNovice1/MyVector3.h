#pragma once
#include <corecrt_math.h>
#include <Novice.h>

class Vector3 final
{
public:
	float x;
	float y;
	float z;
	// コンストラクタ
	Vector3() = default;
	~Vector3() = default;

	Vector3(float x, float y, float z) : x(x), y(y), z(z) {};
	// メンバ関数
	Vector3 operator+(const Vector3& vec) const
	{
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}
	void operator+=(const Vector3& vec)
	{
		x += vec.x, y += vec.y, z += vec.z;
	}
	void operator+=(const float val)
	{
		x += val, y += val, z += val;
	}
	void operator-=(const Vector3& vec)
	{
		x -= vec.x, y -= vec.y, z -= vec.z;
	}
	Vector3 operator-(const Vector3& vec) const
	{
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}
	float operator*(const Vector3& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}
	Vector3 operator*(float val) const
	{
		return Vector3(x * val, y * val, z * val);
	}
	void operator*=(float val)
	{
		x *= val, y *= val, z *= val;
	}
	inline float length()
	{
		return sqrtf(x * x + y * y + z * z);
	}
	inline Vector3 normalize()
	{
		float len = length();
		if (len == 0) return Vector3(0, 0, 0);
		return Vector3(x / len, y / len, z / len);
	}

};