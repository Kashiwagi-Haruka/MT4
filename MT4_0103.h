#pragma once
#include "Function.h"
#include "Quaternion.h"
class MT4_0103 {

	Quaternion quaternion1{2.0f, 3.0, 4.0f, 1.0f};
	Quaternion quaternion2{1.0f, 3.0f, 5.0f, 2.0f};
	Quaternion identity;
	Quaternion conjugate;
	Quaternion inverse;
	Quaternion normalize;
	Quaternion multiply1;
	Quaternion multiply2;
	float norm;

public:
	void Update();
	void Draw();
};
