#include "Quaternion.h"
#include <cmath>


namespace QuaternionFunction {

Quaternion Inverse(const Quaternion& q) {
	float normSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	Quaternion conjugate = Conjugate(q);
	return {conjugate.x / normSq, conjugate.y / normSq, conjugate.z / normSq, conjugate.w / normSq};
}
Quaternion Normalize(const Quaternion& q) {
	float norm = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	return {q.x / norm, q.y / norm, q.z / norm, q.w / norm};
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
Quaternion MakeRotateAxisAngleQuaternion(const struct Vector3& axis, float angle) { Quaternion result{};
	float halfAngle = angle / 2.0f;
	float sinHalfAngle = std::sin(halfAngle);
	result.x = axis.x * sinHalfAngle;
	result.y = axis.y * sinHalfAngle;
	result.z = axis.z * sinHalfAngle;
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
	// 内積を計算
	float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	// もし内積が負なら、q2を反転させて最短経路を取る
	Quaternion q2Copy = q2;
	if (dot < 0.0f) {
		dot = -dot;
		q2Copy.x = -q2.x;
		q2Copy.y = -q2.y;
		q2Copy.z = -q2.z;
		q2Copy.w = -q2.w;
	}
	const float threshold = 0.9995f;
	if (dot > threshold) {
		// クォータニオンが非常に近い場合は線形補間を使用
		Quaternion result{
			q1.x + t * (q2Copy.x - q1.x),
			q1.y + t * (q2Copy.y - q1.y),
			q1.z + t * (q2Copy.z - q1.z),
			q1.w + t * (q2Copy.w - q1.w)};
		return Normalize(result);
	}
	// θを計算
	float theta_0 = std::acos(dot);
	float theta = theta_0 * t;
	// q2を正規化したq1に対して直交する成分を計算
	Quaternion qPerp{
		q2Copy.x - dot * q1.x,
		q2Copy.y - dot * q1.y,
		q2Copy.z - dot * q1.z,
		q2Copy.w - dot * q1.w};
	qPerp = Normalize(qPerp);
	// 最終的なクォータニオンを計算
	Quaternion result{
		q1.x * std::cos(theta) + qPerp.x * std::sin(theta),
		q1.y * std::cos(theta) + qPerp.y * std::sin(theta),
		q1.z * std::cos(theta) + qPerp.z * std::sin(theta),
		q1.w * std::cos(theta) + qPerp.w * std::sin(theta)};
	return result;
	}
} // namespace QuaternionFunction
