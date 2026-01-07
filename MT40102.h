#pragma once
#include "Function.h"
class MT40102 {

	Vector3 from0{};
	Vector3 to0{};
	Vector3 from1{};
	Vector3 to1{};

	Matrix4x4 rotateMatrix0{};
	Matrix4x4 rotateMatrix1{};
	Matrix4x4 rotateMatrix2{};

public:
	void Update();
	void Draw();
};
