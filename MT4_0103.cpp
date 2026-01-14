#include "MT4_0103.h"
#include "Novice.h"

void MT4_0103::Update() { 
	identity = QuaternionFunction::IdentityQuaternion();
	conjugate = QuaternionFunction::Conjugate(quaternion1);
	inverse = QuaternionFunction::Inverse(quaternion1);
	normalize = QuaternionFunction::Normalize(quaternion1);
	multiply1 = QuaternionFunction::Multiply(quaternion1, quaternion2);
	multiply2 = QuaternionFunction::Multiply(quaternion2, quaternion1);
	norm = QuaternionFunction::Norm(quaternion1);

}

void MT4_0103::Draw() {

	Novice::ScreenPrintf(0, 0, "%.2f %.2f %.2f %.2f		: Identity",identity.x,identity.y,identity.z,identity.w);
	Novice::ScreenPrintf(0, 20, "%.2f %.2f %.2f %.2f	: Conjugate",conjugate.x,conjugate.y,conjugate.z,conjugate.w);
	Novice::ScreenPrintf(0, 40, "%.2f %.2f %.2f %.2f	: Inverse",inverse.x,inverse.y,inverse.z,inverse.w);
	Novice::ScreenPrintf(0, 60, "%.2f %.2f %.2f %.2f	: Normalize",normalize.x,normalize.y,normalize.z,normalize.w);
	Novice::ScreenPrintf(0, 80, "%.2f %.2f %.2f %.2f	: Multiply1",multiply1.x,multiply1.y,multiply1.z,multiply1.w);
	Novice::ScreenPrintf(0, 100, "%.2f %.2f %.2f %.2f	: Multiply2",multiply2.x,multiply2.y,multiply2.z,multiply2.w);
	Novice::ScreenPrintf(0, 120, "%.2f					: Norm",norm);


}