#include "MT40102.h"

#include "Novice.h"

void MT40102::Update() {

	from0 = Function::Normalize({1.0f, 0.7f, 0.5f});
	to0.x = from0.x * -1;
	to0.y = from0.y * -1;
	to0.z = from0.z * -1;
	from1 = Function::Normalize({-0.6f, 0.9f, 0.2f});
	to1 = Function::Normalize({0.4f, 0.7f, -0.5f});
	rotateMatrix0 = Function::DirectionToDirection(Function::Normalize({1.0f, 0.0f, 0.0f}), Function::Normalize({-1.0f, 0.0f, 0.0f}));
	rotateMatrix1 = Function::DirectionToDirection(from0, to0);
	rotateMatrix2 = Function::DirectionToDirection(from1, to1);
}

void MT40102::Draw() {
	Novice::ScreenPrintf(0, 0, "rotateMatrix0");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Novice::ScreenPrintf(i * 80, (j + 1) * 40, "%.3f ", rotateMatrix0.m[i][j]);
		}
	}
	Novice::ScreenPrintf(0, 5 * 40, "rotateMatrix1");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Novice::ScreenPrintf(i * 80, (j + 6) * 40, "%.3f ", rotateMatrix1.m[i][j]);
		}
	}
	Novice::ScreenPrintf(0, 11 * 40, "rotateMatrix2");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Novice::ScreenPrintf(i * 80, (j + 12) * 40, "%.3f ", rotateMatrix2.m[i][j]);
		}
	}
}