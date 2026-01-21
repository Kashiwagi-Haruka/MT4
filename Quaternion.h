#pragma once
#include "KamataEngine.h"
using KamataEngine::Vector3;
using KamataEngine::Matrix4x4;
struct Quaternion {

	float x;
	float y;
	float z;
	float w;
};
namespace QuaternionFunction {
Quaternion IdentityQuaternion();
Quaternion Conjugate(const Quaternion& q);
Quaternion Inverse(const Quaternion& q);
Quaternion Normalize(const Quaternion& q);
Quaternion Multiply(const Quaternion& q1, const Quaternion& q2);
float Norm(const Quaternion& q);
Quaternion MakeRotateAxisAngleQuaternion(const struct Vector3& axis, float angle);
Vector3 RotateVector(const Quaternion& q, const Vector3& v);
Vector3 RotateVector(const Vector3& v, const Quaternion& q);
Matrix4x4 MakeRotateMatrix(const Quaternion& q);
Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
}; // namespace QuaternionFunction
