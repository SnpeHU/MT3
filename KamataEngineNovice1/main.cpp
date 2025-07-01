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

struct Line {
	Vector3 origin; // 始点
	Vector3 diff; // 方向ベクトル
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment
{
	Vector3 origin;
	Vector3 diff;
};

struct Plane
{
	Vector3 normal; // 法線ベクトル
	float distance;
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

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix,int color) {
	const uint32_t kCircleSegments = 36; // 円の分割数
	const float kLonStep = 2.0f * pi_v<float> / 24; // 経度分割一つ分の角度
	const float kLatStep = 2.0f * pi_v<float> / 24;  // 緯度分割一つ分の角度

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

			Novice::DrawLine(int(sp0.x), int(sp0.y), int(sp1.x), int(sp1.y), color);
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

			Novice::DrawLine(int(sp0.x), int(sp0.y), int(sp1.x), int(sp1.y), color);
		}
	}
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 center = plane.normal * plane.distance;
	Vector3 perpendicular[4];
	perpendicular[0] = plane.normal.Perpendicular().normalize();
	perpendicular[1] = { -perpendicular[0].x,-perpendicular[0].y,-perpendicular[0].z };
	perpendicular[2] = Cross(perpendicular[0], plane.normal);
	perpendicular[3] = { -perpendicular[2].x,-perpendicular[2].y,-perpendicular[2].z };

	Vector3 points[4];
	for (int32_t index = 0;index < 4; ++index) {
		Vector3 extend = perpendicular[index] * 2.0f; // 平面の大きさを調整
		Vector3 point = center + extend;
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[1].x), int(points[1].y), color);


}

Vector3 Project(const Vector3& v1,const Vector3& v2)
{
	// v2の長さ
	float length = std::sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
	if (length == 0.0f) {
		return { 0.0f, 0.0f, 0.0f }; // ゼロベクトルの場合は投影できないのでゼロベクトルを返す
	}
	// v2を単位ベクトルにする
	Vector3 unitV2 = { v2.x / length, v2.y / length, v2.z / length };
	// v1をv2に投影する
	float dotProduct = v1.x * unitV2.x + v1.y * unitV2.y + v1.z * unitV2.z;
	return { unitV2.x * dotProduct, unitV2.y * dotProduct, unitV2.z * dotProduct };
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
	// セグメントの始点から終点へのベクトル
	Vector3 segmentVector = { segment.diff.x - segment.origin.x, segment.diff.y - segment.origin.y, segment.diff.z - segment.origin.z };
	// セグメントの長さの二乗
	float segmentLengthSquared = segmentVector.x * segmentVector.x + segmentVector.y * segmentVector.y + segmentVector.z * segmentVector.z;
	if (segmentLengthSquared == 0.0f) {
		// セグメントがゼロ長の場合、始点を返す
		return segment.origin;
	}
	// 点からセグメントの始点へのベクトル
	Vector3 pointToOrigin = { point.x - segment.origin.x, point.y - segment.origin.y, point.z - segment.origin.z };
	// セグメント上の点までの投影係数
	float t = (pointToOrigin.x * segmentVector.x + pointToOrigin.y * segmentVector.y + pointToOrigin.z * segmentVector.z) / segmentLengthSquared;
	if (t < 0.0f) {
		// 投影が始点より前の場合、始点を返す
		return segment.origin;
	}
	else if (t > 1.0f) {
		// 投影が終点より後の場合、終点を返す
		return { segment.diff.x,segment.diff.y,segment.diff.z };
	}
	else {
		// セグメント上の最近傍点を計算して返す
		return {
			segment.origin.x + t * segmentVector.x,
			segment.origin.y + t * segmentVector.y,
			segment.origin.z + t * segmentVector.z
		};
	}
}

float Length(const Vector3& v1, const Vector3& v2)
{
	Vector3 diff = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}

bool IsCollision(const Sphere& sphereA, const Sphere& sphereB) {
	float distance = Length(sphereA.center, sphereB.center);
	float radiusSum = sphereA.radius + sphereB.radius;
	return distance <= radiusSum;
}

bool IsCollision(const Sphere& sphere, const Plane& plane) {
	Vector3 normal = { plane.normal.x, plane.normal.y, plane.normal.z };
	Vector3 toSphere = { sphere.center.x - (normal.x * plane.distance), sphere.center.y - (normal.y * plane.distance), sphere.center.z - (normal.z * plane.distance) };
	float distance = std::sqrt(toSphere.x * toSphere.x + toSphere.y * toSphere.y + toSphere.z * toSphere.z);
	return distance <= sphere.radius;
}

bool IsCollision(const Segment& segment, const Plane& plane) {

	return false;
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


	Vector3 position1{ -1.5f,0.0f,1.5f };
	Vector3 position2{ 0.0f,0.0f,0.0f };

	
	int color = WHITE;

	Segment segment{ position1, position2 }; // セグメントの始点と終点

	Plane plane{ {0.0f,1.0f,0.0f}, 0.0f }; // 平面の法線と距離


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
		if (IsCollision(segment, plane)) {
			color = RED;
		}
		else {
			color = WHITE;
		}

		ImGui::Begin("Plane");
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("Plane.Distance", &plane.distance, 0.01f);
		ImGui::End();
		plane.normal = plane.normal.normalize(); // 法線ベクトルを正規化
		///
		/// ↓描画処理ここから
		///
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		DrawPlane(plane, worldViewProjectionMatrix, viewportMatrix, color);
	



		ImGui::Begin("Sphere");
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

