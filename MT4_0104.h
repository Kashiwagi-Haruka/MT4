#pragma once
#include "Quaternion.h"
class MT4_0104 {

	Quaternion rotation{};
	Vector3 pointY{2.1f, -0.9f, 1.3f};
	Matrix4x4 rotateMatrix{};
	Vector3 rotateByQuaternion{};
	Vector3 rotateByMatrix{};

public:
	void Update();
	void Draw();

};
