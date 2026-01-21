#include "MT40105.h"
#include "Novice.h"

void MT40105::Update(){ 	
	rotation0 = QuaternionFunction::MakeRotateAxisAngleQuaternion({0.71f, 0.71f, 0.0f}, 0.3f);
	rotation1 = QuaternionFunction::MakeRotateAxisAngleQuaternion({0.71f, 0.0f, 0.71f}, 3.141592f);
	
	interpolate0 = QuaternionFunction::Slerp(rotation0, rotation1, 0.0f);
	interpolate1 = QuaternionFunction::Slerp(rotation0, rotation1, 0.3f);
	interpolate2 = QuaternionFunction::Slerp(rotation0, rotation1, 0.5f);
	interpolate3 = QuaternionFunction::Slerp(rotation0, rotation1, 0.7f);
	interpolate4 = QuaternionFunction::Slerp(rotation0, rotation1, 1.0f);
}
void MT40105::Draw(){
	Novice::ScreenPrintf(0, 0,   "%.2f %.2f %.2f %.2f	:	interpolate0, Slerp(q0,q1,0.0f)", interpolate0.x, interpolate0.y, interpolate0.z, interpolate0.w);
	Novice::ScreenPrintf(0, 20,  "%.2f %.2f %.2f %.2f	:	interpolate1, Slerp(q0,q1,0.3f)", interpolate1.x, interpolate1.y, interpolate1.z, interpolate1.w);
	Novice::ScreenPrintf(0, 40,  "%.2f %.2f %.2f %.2f	:	interpolate2, Slerp(q0,q1,0.5f)", interpolate2.x, interpolate2.y, interpolate2.z, interpolate2.w);
	Novice::ScreenPrintf(0, 80,  "%.2f %.2f %.2f %.2f	:	interpolate3, Slerp(q0,q1,0.7f)", interpolate3.x, interpolate3.y, interpolate3.z, interpolate3.w);
	Novice::ScreenPrintf(0, 100, "%.2f %.2f %.2f %.2f	:	interpolate4, Slerp(q0,q1,1.0f)", interpolate4.x, interpolate4.y, interpolate4.z, interpolate4.w);
}