#include "MT4_0104.h"
#include "Novice.h"
#include "Function.h"
void MT4_0104::Update(){ 	
	rotation = QuaternionFunction::MakeRotateAxisAngleQuaternion(Function::Normalize({1.0f, 0.4f, -0.2f}), 0.45f);
	rotateMatrix = QuaternionFunction::MakeRotateMatrix(rotation);
	
	rotateByQuaternion = QuaternionFunction::RotateVector(pointY,rotation);
	rotateByMatrix = Function::TransformVM(pointY, rotateMatrix);

}

void MT4_0104::Draw(){

	Novice::ScreenPrintf(0, 0, "rotation %.2f %.2f %.2f %.2f", rotation.x, rotation.y, rotation.z, rotation.w);
	Novice::ScreenPrintf(0, 20, "rotateMatrix");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Novice::ScreenPrintf(i * 60, (j + 2) * 20, "%.3f ", rotateMatrix.m[i][j]);
		}
	}
	Novice::ScreenPrintf(0, 120, "rotateByQuaternion %.2f %.2f %.2f", rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z);
	Novice::ScreenPrintf(0, 140, "rotateByMatrix     %.2f %.2f %.2f", rotateByMatrix.x, rotateByMatrix.y, rotateByMatrix.z);
}