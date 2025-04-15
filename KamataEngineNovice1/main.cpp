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



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 translate = {4.1f, 2.6f, 0.8f };
	Vector3 scale = { 1.5f, 5.2f, 7.3f };

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Vector3 point = { 2.3f, 3.8f,1.4f };

	Matrix4x4 m1 = {1.0f,2.0f,3.0f,4.0f,
					3.0f,1.0f,1.0f,2.0f,
					1.0f,4.0f,2.0f,3.0f,
					2.0f,2.0f,1.0f,3.0f};

	Vector3 transformed = Transform(point, translateMatrix);



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
		MatrixScreenPrint(translateMatrix, 0, 0, "Translate");
		MatrixScreenPrint(scaleMatrix, 0, 5 * kRowHeight, "Scale");
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

