#include <Novice.h>
#include "MyVector3.h"
#include "MyMatrix4x4.h"
#include <numbers>
#include <imgui.h>
using namespace std::numbers;

const char kWindowTitle[] = "GC2A_04_コウ_ホウケイ_タイトル";

const int kRowHeight = 20;
const int kColumnWidth = 60;

int kWindowWidth = 1280;
int kWindowHeight = 720;


struct Sphere
{
	Vector3 center;
	float radius;
};

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

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfSize = 2.0f;
	const uint32_t kSubdivisions = 10;// グリッドの分割数
	const float kGridEvery = kGridHalfSize * 2 / (float)kSubdivisions;

	//奥から手前への線
	for (uint32_t xIndex = 0;xIndex <= kSubdivisions;xIndex++) {
		float x = -kGridHalfSize + kGridEvery * xIndex;
		Vector3 start = Transform({ x, 0.0f, -kGridHalfSize }, viewProjectionMatrix);
		Vector3 end = Transform({ x, 0.0f, kGridHalfSize }, viewProjectionMatrix);
		start = Transform(start, viewportMatrix);
		end = Transform(end, viewportMatrix);
		if (xIndex == kSubdivisions / 2) {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xFF0000FF);
		}
		else
		{ 
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xAAAAAAFF);
		}
	}
	//左右の線
	for (uint32_t zIndex = 0;zIndex <= kSubdivisions;zIndex++) {
		float z = -kGridHalfSize + kGridEvery * zIndex;
		Vector3 start = Transform({ -kGridHalfSize, 0.0f, z }, viewProjectionMatrix);
		Vector3 end = Transform({ kGridHalfSize, 0.0f, z }, viewProjectionMatrix);
		start = Transform(start, viewportMatrix);
		end = Transform(end, viewportMatrix);
		if (zIndex == kSubdivisions / 2) {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xFF0000FF);
		}
		else
		{
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xAAAAAAFF);
		}
		
	}

}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const uint32_t kCircleSegments = 36; // 円の分割数
	const float kLonStep = 2.0f * pi_v<float> / kCircleSegments; // 経度分割一つ分の角度
	const float kLatStep = 2.0f * pi_v<float> / kCircleSegments;  // 緯度分割一つ分の角度

	for (uint32_t lat = 0; lat <= kCircleSegments; ++lat) {
		float theta = -pi_v<float> / 2.0f + kLatStep * lat; // 緯度角
		for (uint32_t lon = 0; lon < kCircleSegments; ++lon) {
			float phi0 = kLonStep * lon;
			float phi1 = kLonStep * (lon + 1);


			Vector3 p0{
				sphere.center.x + sphere.radius * std::cos(theta) * std::cos(phi0),
				sphere.center.y + sphere.radius * std::sin(theta),
				sphere.center.z + sphere.radius * std::cos(theta) * std::sin(phi0)
			};

			Vector3 p1{
				sphere.center.x + sphere.radius * std::cos(theta) * std::cos(phi1),
				sphere.center.y + sphere.radius * std::sin(theta),
				sphere.center.z + sphere.radius * std::cos(theta) * std::sin(phi1)
			};


			Vector3 sp0 = Transform(Transform(p0, viewProjectionMatrix), viewportMatrix);
			Vector3 sp1 = Transform(Transform(p1, viewProjectionMatrix), viewportMatrix);

			Novice::DrawLine(int(sp0.x), int(sp0.y), int(sp1.x), int(sp1.y), 0xFFFFFFFF);
		}
	}

	
	for (uint32_t lon = 0; lon < kCircleSegments; ++lon) {
		float phi1 = kLonStep * lon;
		for (uint32_t lat = 0; lat < kCircleSegments; ++lat) {
			float theta0 = -pi_v<float> / 2.0f + kLatStep * lat;
			float theta1 = -pi_v<float> / 2.0f + kLatStep * (lat + 1);


			Vector3 p0{
				sphere.center.x + sphere.radius * std::cos(theta0) * std::cos(phi1),
				sphere.center.y + sphere.radius * std::sin(theta0),
				sphere.center.z + sphere.radius * std::cos(theta0) * std::sin(phi1)
			};

			Vector3 p1{
				sphere.center.x + sphere.radius * std::cos(theta1) * std::cos(phi1),
				sphere.center.y + sphere.radius * std::sin(theta1),
				sphere.center.z + sphere.radius * std::cos(theta1) * std::sin(phi1)
			};

			Vector3 sp0 = Transform(Transform(p0, viewProjectionMatrix), viewportMatrix);
			Vector3 sp1 = Transform(Transform(p1, viewProjectionMatrix), viewportMatrix);

			Novice::DrawLine(int(sp0.x), int(sp0.y), int(sp1.x), int(sp1.y), 0xFFFFFFFF);
		}
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	//
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };

	Vector3 cameraPostion{ 0.0f,4.0f,-10.0f };
	Vector3 cameraRotate{ 0.3f,0.0f,0.0f };

	
	//Vector3 kLocalVertices[3] = {
	//	{ -1.0f, -1.0f, 0.0f },
	//	{  1.0f, -1.0f, 0.0f },
	//	{  0.0f,  1.0f, 0.0f }
	//};
	//Vector3 screenVertices[3];

	Sphere sphere;
	sphere.center = { 0.0f, 0.0f, 0.0f };
	sphere.radius = 1.0f;

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
				//回転
		Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraPostion);
		Matrix4x4 viewMatrix = cameraMatrix.Inverse();
		Matrix4x4 projectionMatrix = MakePerspectiveMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		
		///
		/// ↑更新処理ここまで
		///
		

		//cameraRotate.z += 0.01f; // カメラのY軸回転を更新
		//rotate.y += 0.02f;

		///
		/// ↓描画処理ここから
		///
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix);
		ImGui::Begin("Sphere");
		ImGui::DragFloat3("Center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("Radius", &sphere.radius, 0.01f);
		ImGui::End();
		ImGui::Begin("Camera");
		ImGui::DragFloat3("Position", &cameraPostion.x, 0.01f);
		ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.01f);
		ImGui::End();
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

