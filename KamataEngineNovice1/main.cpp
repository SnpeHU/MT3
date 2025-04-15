#include <Novice.h>
#include "Vector3.h"
#include <corecrt_math.h>

const char kWindowTitle[] = "GC1A_05_コウ_ホウケイ_タイトル";

Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
Vector3 Multiply(float scalar, const Vector3& v2)
{
	return Vector3(scalar * v2.x, scalar * v2.y, scalar * v2.z);
}
float Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
float Length(const Vector3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
Vector3 Normalize(const Vector3& v)
{
	float length = Length(v);
	return Vector3(v.x / length, v.y / length, v.z / length);
}

void VectorScreenPrint(int x,int y,const Vector3& vector,const char* label)
{
	Novice::ScreenPrintf(x, y, "(%.2f, %.2f, %.2f) %s", vector.x, vector.y, vector.z, label);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 v1{ 1.0f,3.0f,-5.0f };
	Vector3 v2{ 4.0f,-1.0f,2.0f };
	float k = 4.0f;

	Vector3 resultAdd = Add(v1, v2);
	Vector3 resultSubtract = Subtract(v1, v2);
	Vector3 resultMultiply = Multiply(k, v1);
	float resultDot = Dot(v1, v2);
	float resultLength = Length(v1);
	Vector3 resultNormalize = Normalize(v2);

	int kRowHeight = 20;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		VectorScreenPrint(10, 10, resultAdd, "Add:");
		VectorScreenPrint(10, 10 + kRowHeight, resultSubtract, "Subtract:");
		VectorScreenPrint(10, 10 + kRowHeight * 2, resultMultiply, "Multiply:");
		Novice::ScreenPrintf(10, 10 + kRowHeight * 3, "Dot: %.2f", resultDot);
		Novice::ScreenPrintf(10, 10 + kRowHeight * 4, "Length: %.2f", resultLength);
		VectorScreenPrint(10, 10 + kRowHeight * 5, resultNormalize, "Normalize:");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

