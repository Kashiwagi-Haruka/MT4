#include "Quaternion.h"
#include "Function.h"
#include <algorithm>
#include <cmath>

namespace QuaternionFunction {

Quaternion Inverse(const Quaternion& q) {
	float normSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	Quaternion conjugate = Conjugate(q);
	return {conjugate.x / normSq, conjugate.y / normSq, conjugate.z / normSq, conjugate.w / normSq};
}
Quaternion Normalize(const Quaternion& q) {
	double norm = std::sqrt(static_cast<double>(q.x) * q.x + static_cast<double>(q.y) * q.y + static_cast<double>(q.z) * q.z + static_cast<double>(q.w) * q.w);
	if (norm <= 1e-12) {
		return {0.0f, 0.0f, 0.0f, 1.0f};
	}
	return {static_cast<float>(q.x / norm), static_cast<float>(q.y / norm), static_cast<float>(q.z / norm), static_cast<float>(q.w / norm)};
}
Quaternion Multiply(const Quaternion& q1, const Quaternion& q2) {
	return {
	    q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y, q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x, q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
	    q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z};
}
float Norm(const Quaternion& q) {
	float norm = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	return {norm};
}
Quaternion IdentityQuaternion() { return {0.0f, 0.0f, 0.0f, 1.0f}; }
Quaternion Conjugate(const Quaternion& q) { return {-q.x, -q.y, -q.z, q.w}; }
Quaternion MakeRotateAxisAngleQuaternion(const struct Vector3& axis, float angle) {
	Quaternion result{};
	Vector3 normalizedAxis = Function::Normalize(axis);
	float halfAngle = angle / 2.0f;
	float sinHalfAngle = std::sin(halfAngle);
	result.x = normalizedAxis.x * sinHalfAngle;
	result.y = normalizedAxis.y * sinHalfAngle;
	result.z = normalizedAxis.z * sinHalfAngle;
	result.w = std::cos(halfAngle);
	return result;
}
Vector3 RotateVector(const Quaternion& q, const Vector3& v) {
	// q * v * q^-1 で回転
	Quaternion vq = {v.x, v.y, v.z, 0.0f};
	Quaternion qInv = Inverse(q);
	Quaternion result = Multiply(Multiply(q, vq), qInv);
	return {result.x, result.y, result.z};
}
Vector3 RotateVector(const Vector3& v, const Quaternion& q) {
	return RotateVector(q, v);
}
Matrix4x4 MakeRotateMatrix(const Quaternion& q){ Matrix4x4 result{};
	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;
	result.m[0][0] = 1.0f - 2.0f * (yy + zz);
	result.m[0][1] = 2.0f * (xy - wz);
	result.m[0][2] = 2.0f * (xz + wy);
	result.m[0][3] = 0.0f;
	result.m[1][0] = 2.0f * (xy + wz);
	result.m[1][1] = 1.0f - 2.0f * (xx + zz);
	result.m[1][2] = 2.0f * (yz - wx);
	result.m[1][3] = 0.0f;
	result.m[2][0] = 2.0f * (xz - wy);
	result.m[2][1] = 2.0f * (yz + wx);
	result.m[2][2] = 1.0f - 2.0f * (xx + yy);
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}
Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) {
	Quaternion q1Norm = Normalize(q1);
	Quaternion q2Norm = Normalize(q2);
	// 内積を計算
	double dot = static_cast<double>(q1Norm.x) * q2Norm.x + static_cast<double>(q1Norm.y) * q2Norm.y + static_cast<double>(q1Norm.z) * q2Norm.z + static_cast<double>(q1Norm.w) * q2Norm.w;
	dot = std::clamp(dot, -1.0, 1.0);
	// もし内積が負なら、q2を反転させて最短経路を取る
	Quaternion q2Copy = q2Norm;
	if (dot <= 0.0) {
		dot = -dot;
		q2Copy.x = -q2Norm.x;
		q2Copy.y = -q2Norm.y;
		q2Copy.z = -q2Norm.z;
		q2Copy.w = -q2Norm.w;
	}
	q2Copy = Normalize(q2Copy);
	if (dot > 0.9995) {
		Quaternion result{q1Norm.x + (q2Copy.x - q1Norm.x) * t, q1Norm.y + (q2Copy.y - q1Norm.y) * t, q1Norm.z + (q2Copy.z - q1Norm.z) * t, q1Norm.w + (q2Copy.w - q1Norm.w) * t};
		return Normalize(result);
	}
	double theta0 = std::acos(dot);
	double theta = theta0 * t;
	double sinTheta0 = std::sin(theta0);
	double sinTheta = std::sin(theta);
	double s0 = std::cos(theta) - dot * sinTheta / sinTheta0;
	double s1 = sinTheta / sinTheta0;
	Quaternion result{
	    static_cast<float>(q1Norm.x * s0 + q2Copy.x * s1), static_cast<float>(q1Norm.y * s0 + q2Copy.y * s1), static_cast<float>(q1Norm.z * s0 + q2Copy.z * s1),
	    static_cast<float>(q1Norm.w * s0 + q2Copy.w * s1)};
	return Normalize(result);
}
} // namespace QuaternionFunction
