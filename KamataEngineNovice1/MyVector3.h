#pragma once
class MyVector3 final
{
public:
	float x;
	float y;
	float z;
	// コンストラクタ
	MyVector3() = default;
	MyVector3(float x, float y, float z) : x(x), y(y), z(z) {};
	// メンバ関数
};