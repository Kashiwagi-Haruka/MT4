
#include "Function.h"
#include <cassert>
#include <cmath>


static const int kColumnWidth = 60;
static const int kRowHeight = 30;

namespace Function {


float Length(const Vector3& v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

// direction = 向きたい方向（正規化推奨）
// forwardAxis = モデルの前方向（Cube は X軸 {1,0,0}）
// return = 回転角（x, y, z）
Vector3 DirectionToRotation(const Vector3& direction) {
	// 方向を正規化
	Vector3 dir = Normalize(direction);

	// --- Yaw（水平回転）---
	// atan2(y, x) ではなく、x, z を使用
	float yaw = std::atan2(dir.z, dir.x);

	// --- Pitch（上下の角度）---
	float horizontalLen = std::sqrt(dir.x * dir.x + dir.z * dir.z);
	float pitch = std::atan2(dir.y, horizontalLen);

	// Roll は不要なので 0
	float roll = 0.0f;

	return {pitch, -yaw, roll};
}



Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result{};
	float f = 1.0f / std::tanf(fovY * 0.5f);
	result.m[0][0] = f / aspectRatio;
	result.m[1][1] = f;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	result.m[3][3] = 0.0f;
	return result;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result{};
	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = 1.0f / (farClip - nearClip);
	result.m[3][0] = -(right + left) / (right - left);
	result.m[3][1] = -(top + bottom) / (top - bottom);
	result.m[3][2] = -nearClip / (farClip - nearClip);
	result.m[3][3] = 1.0f;
	return result;
}
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {

	Matrix4x4 result{};
	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -height / 2.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + width / 2.0f;
	result.m[3][1] = top + height / 2.0f;
	result.m[3][2] = minDepth;
	result.m[3][3] = 1.0f;
	return result;
}


Matrix4x4 MakeTranslateMatrix(Vector3 translate) {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}
Matrix4x4 Function::MakeTranslateMatrix(float x, float y, float z) {
	Matrix4x4 result{};

	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;

	result.m[3][0] = x;
	result.m[3][1] = y;
	result.m[3][2] = z;

	return result;
}

Matrix4x4 MakeScaleMatrix(Vector3 scale) {
	Matrix4x4 result{};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = std::cosf(radian);
	result.m[1][2] = std::sinf(radian);
	result.m[2][1] = -std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}


Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = std::cosf(radian);
	result.m[0][2] = -std::sinf(radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = std::cosf(radian);
	result.m[0][1] = std::sinf(radian);
	result.m[1][0] = -std::sinf(radian);
	result.m[1][1] = std::cosf(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int row = 0; row < 4; ++row) {
		for (int colum = 0; colum < 4; ++colum) {
			result.m[row][colum] = m1.m[row][0] * m2.m[0][colum] + m1.m[row][1] * m2.m[1][colum] + m1.m[row][2] * m2.m[2][colum] + m1.m[row][3] * m2.m[3][colum];
		}
	}
	return result;
}


Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) {
	Matrix4x4 result{};

	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZ = Multiply(rotateX, Multiply(rotateY, rotateZ));

	Matrix4x4 ScaleRotateMatrix = Multiply(MakeScaleMatrix(scale), rotateXYZ);
	result = Multiply(ScaleRotateMatrix, MakeTranslateMatrix(translate));

	return result;
}

Vector3 TransformVM(const Vector3& v, const Matrix4x4& m) {
	Vector3 result;
	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3];
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3];
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3];
	return result;
}

Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 i;

	float d =
	    m.m[0][0] *
	        (m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1])) -
	    m.m[0][1] *
	        (m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0])) +
	    m.m[0][2] *
	        (m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) -
	    m.m[0][3] *
	        (m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

	i.m[0][0] =
	    (m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1])) / d;
	i.m[0][1] =
	    -(m.m[0][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[0][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1])) / d;
	i.m[0][2] =
	    (m.m[0][1] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][2] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) + m.m[0][3] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1])) / d;
	i.m[0][3] =
	    -(m.m[0][1] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][2] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) + m.m[0][3] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1])) / d;

	i.m[1][0] =
	    -(m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0])) / d;
	i.m[1][1] =
	    (m.m[0][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0])) / d;
	i.m[1][2] =
	    -(m.m[0][0] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][1] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0])) / d;
	i.m[1][3] =
	    (m.m[0][0] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][1] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][2] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0])) / d;

	i.m[2][0] =
	    (m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) / d;
	i.m[2][1] =
	    -(m.m[0][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) / d;
	i.m[2][2] =
	    (m.m[0][0] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0])) / d;
	i.m[2][3] =
	    -(m.m[0][0] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0])) / d;

	i.m[3][0] =
	    -(m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) / d;
	i.m[3][1] =
	    (m.m[0][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[0][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) / d;
	i.m[3][2] =
	    -(m.m[0][0] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0]) + m.m[0][2] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0])) / d;
	i.m[3][3] =
	    (m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) + m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0])) / d;

	return i;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

Vector3 Normalize(const Vector3& v) {
	float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (len > 1e-6f)
		return {v.x / len, v.y / len, v.z / len};
	else
		return {0.0f, 0.0f, 0.0f};
}
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
	Vector3 n = Function::Normalize(axis);

	float nx = n.x;
	float ny = n.y;
	float nz = n.z;

	float c = std::cos(angle);
	float s = std::sin(angle);
	float one_c = 1.0f - c;

	Matrix4x4 R{};


	R.m[0][0] = nx * nx * one_c + c;
	R.m[0][1] = nx * ny * one_c - nz * s;
	R.m[0][2] = nx * nz * one_c + ny * s;
	R.m[0][3] = 0.0f;


	R.m[1][0] = ny * nx * one_c + nz * s;
	R.m[1][1] = ny * ny * one_c + c;
	R.m[1][2] = ny * nz * one_c - nx * s;
	R.m[1][3] = 0.0f;

	R.m[2][0] = nz * nx * one_c - ny * s;
	R.m[2][1] = nz * ny * one_c + nx * s;
	R.m[2][2] = nz * nz * one_c + c;
	R.m[2][3] = 0.0f;

	R.m[3][0] = 0.0f;
	R.m[3][1] = 0.0f;
	R.m[3][2] = 0.0f;
	R.m[3][3] = 1.0f;

	return R;
}


Matrix4x4 MakeIdentity4x4() {

	Matrix4x4 result;

	result = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

	return result;
}
float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;}

Vector3 Distance(const Vector3& pos1, const Vector3& pos2) { return {pos2.x - pos1.x, pos2.y - pos1.y, pos2.z - pos1.z}; }
Matrix4x4 DirectionToDirection(Vector3 from, Vector3 to) {
	// --- 正規化 ---
	Vector3 u = Normalize(from);
	Vector3 v = Normalize(to);

	float cosTheta = Dot(u, v);

	// ==============================
	// 同方向（回転なし）
	// ==============================
	if (cosTheta > 0.9999f) {
		return MakeIdentity4x4();
	}

	// ==============================
	// 逆方向（180度回転）
	// ==============================
	if (cosTheta < -0.9999f) {
		Vector3 n;

		// u に垂直な軸を「成分から決める」
		if (fabs(u.x) > 1e-6f || fabs(u.y) > 1e-6f) {
			// ( uy, -ux, 0 )
			n = Vector3{u.y, -u.x, 0.0f};
		} else {
			// ( uz, 0, -ux )
			n = Vector3{u.z, 0.0f, -u.x};
		}

		n = Normalize(n);

		float x = n.x;
		float y = n.y;
		float z = n.z;

		// 180度回転行列（Row-major用 → 最後にTranspose）
		Matrix4x4 R = MakeIdentity4x4();

		R.m[0][0] = 2 * x * x - 1;
		R.m[0][1] = 2 * x * y;
		R.m[0][2] = 2 * x * z;

		R.m[1][0] = 2 * y * x;
		R.m[1][1] = 2 * y * y - 1;
		R.m[1][2] = 2 * y * z;

		R.m[2][0] = 2 * z * x;
		R.m[2][1] = 2 * z * y;
		R.m[2][2] = 2 * z * z - 1;

		return Transpose(R);
	}

	// ==============================
	// 通常ケース（Rodrigues）
	// ==============================
	Vector3 crossUV = Cross(u, v);
	Vector3 n = Normalize(crossUV);

	float sinTheta = Length(crossUV);

	float x = n.x;
	float y = n.y;
	float z = n.z;

	float c = cosTheta;
	float s = sinTheta;
	float t = 1.0f - c;

	Matrix4x4 R;

	R.m[0][0] = x * x * t + c;
	R.m[0][1] = x * y * t + z * s;
	R.m[0][2] = x * z * t - y * s;
	R.m[0][3] = 0.0f;

	R.m[1][0] = y * x * t - z * s;
	R.m[1][1] = y * y * t + c;
	R.m[1][2] = y * z * t + x * s;
	R.m[1][3] = 0.0f;

	R.m[2][0] = z * x * t + y * s;
	R.m[2][1] = z * y * t - x * s;
	R.m[2][2] = z * z * t + c;
	R.m[2][3] = 0.0f;

	R.m[3][0] = 0.0f;
	R.m[3][1] = 0.0f;
	R.m[3][2] = 0.0f;
	R.m[3][3] = 1.0f;

	return Transpose(R);
}
} // namespace Function
Vector3 operator+ (const Vector3& v1, const Vector3& v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }
Vector3 operator-(const Vector3& v1, const Vector3& v2) {return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};}
Vector3 operator*(const Vector3& v, float scalar) { return {v.x * scalar, v.y * scalar, v.z * scalar}; }
Vector3 operator+=(Vector3& v1, const Vector3& v2) { return v1 = v1 + v2; }