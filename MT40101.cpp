#include "MT40101.h"
#include "Novice.h"
#include "Function.h"

void MT40101::Update() {
	
	Vector3 axis = {1.0f, 1.0f, 1.0f};
	float angle = 0.44f;

	result = Function::MakeRotateAxisAngle(axis, angle);

}

void MT40101::Draw() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Novice::ScreenPrintf(i * 60, j * 40, "%.3f ", result.m[i][j]);
		}
	
	}


}
