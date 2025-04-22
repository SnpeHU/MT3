#include <Novice.h>
#include "MyVector3.h"
#include "MyMatrix4x4.h"

const char kWindowTitle[] = "GC2A_04_コウ_ホウケイ_タイトル";

const int kRowHeight = 20;
const int kColumnWidth = 60;

void MatrixScreenPrint(const Matrix4x4& matrix, int x, int y,const char* label) {

	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth,y + row * kRowHeight,"%6.02f",matrix.m[row][column]);
		}
	}
	Novice::ScreenPrintf(x + 4 * kColumnWidth, y, "%s", label);
}

void VectorScreenPrint(int x, int y, const Vector3& vector, const char* label)
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

	Vector3 rotate = { 0.4f,1.43f,-0.8f };
	Vector3 scale = { 1.2f,0.79f,-2.1f };
	Vector3 translate = { 2.7f,-4.15f,1.57f };

	Matrix4x4 worldMatrix = MakeAffineMatrix( scale, rotate, translate);

	Matrix4x4 orthographicMatrix = MakeOrthographicMatrix(-160.f, 160.f, 200.f, 300.0f, 0.0f, 1000.0f);
	Matrix4x4 perspectiveMatrix = MakePerspectiveMatrix(0.63f, 1.33f, 0.1f, 1000.0f);
	Matrix4x4 viewportMatrix = MakeViewportMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);

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
		MatrixScreenPrint(orthographicMatrix, 0, 0, "orthographicMatrix");
		MatrixScreenPrint(perspectiveMatrix, 0, kRowHeight * 5, "perspectiveMatrix");
		MatrixScreenPrint(viewportMatrix, 0, kRowHeight * 10, "viewportMatrix");
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

