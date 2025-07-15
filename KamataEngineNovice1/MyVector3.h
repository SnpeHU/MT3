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
	inline float length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}
	inline Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
		return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	inline Vector3 Normalize(const Vector3& v) {
		float len = v.length();
		if (len == 0) return Vector3(0, 0, 0);
		return Vector3(v.x / len, v.y / len, v.z / len);
	}

	inline Vector3 normalize() const
	{
		float len = length();
		if (len == 0) return Vector3(0, 0, 0);
		return Vector3(x / len, y / len, z / len);
	}

	inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
		return Vector3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		);
	}

	inline Vector3 Perpendicular() const {
		if (x != 0 || y != 0) {
			return Vector3(-y, x, 0); // XY平面上での垂直ベクトル
		}
		else if (y != 0 || z != 0) {
			return Vector3(0, -z, y); // YZ平面上での垂直ベクトル
		}
		else {
			return Vector3(0, 0, 1); // Z軸に沿った垂直ベクトル
		}

	}



};

inline float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
