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

	Matrix4x4 m1 = {3.2f,0.7f,9.6f,4.4f,
					5.5f,1.3f,7.8f,2.1f,
					6.9f,8.0f,2.6f,1.0f,
					0.5f,7.2f,5.1f,3.3f};

	Matrix4x4 m2 = { 4.1f,6.5f,3.3f,2.2f,
					8.8f,0.6f,9.9f,7.7f,
					1.1f,5.5f,6.6f,0.0f,
					3.3f,9.9f,8.8f,2.2f };



	Matrix4x4 resultAdd = m1.Add(m2);
	Matrix4x4 resultSub = m1.Subtract(m2);
	Matrix4x4 resultMul = m1.Multiply(m2);
	Matrix4x4 InverseM1 = m1.Inverse();
	Matrix4x4 InverseM2 = m2.Inverse();
	Matrix4x4 transposeM1 = m1.Transpose();
	Matrix4x4 transposeM2 = m2.Transpose();
	Matrix4x4 identityM1;
	identityM1 = identityM1.MakeIdentity4x4();


 

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
		MatrixScreenPrint(resultAdd, 0, 0,"Add");
		MatrixScreenPrint(resultSub, 0, kRowHeight * 5,"Sub");
		MatrixScreenPrint(resultMul, 0, kRowHeight * 10, "Mul");
		MatrixScreenPrint(InverseM1, 0, kRowHeight * 15, "InverseM1");
		MatrixScreenPrint(InverseM2, 0, kRowHeight * 20, "InverseM2");
		MatrixScreenPrint(transposeM1, kColumnWidth * 5, kRowHeight * 5, "TransposeM1");
		MatrixScreenPrint(transposeM2, kColumnWidth * 5, kRowHeight * 10, "TransposeM2");
		MatrixScreenPrint(identityM1, kColumnWidth * 5, kRowHeight * 15, "IdentityM1");

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

