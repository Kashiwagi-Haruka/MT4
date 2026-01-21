#pragma once
#include "Quaternion.h"
class MT40105 {

	Quaternion rotation0;
	Quaternion rotation1;

	Quaternion interpolate0;
	Quaternion interpolate1;
	Quaternion interpolate2;
	Quaternion interpolate3;
	Quaternion interpolate4;



	public:
	void Update();
	void Draw();
};
