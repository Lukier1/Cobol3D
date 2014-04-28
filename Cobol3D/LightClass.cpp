#include "LightClass.h"

namespace Cobol {
	LightClass::LightClass() {
		mDiffuseColor = XMFLOAT4(0,0,0,0);
		mDirection = XMFLOAT3(0,0,0);
		mAmbient = XMFLOAT4(0,0,0,0);
	}
	LightClass::LightClass(const LightClass& source)
	{

	}
	LightClass::~LightClass() {
	}
	void LightClass::SetSpecularColor(XMFLOAT4 color)
	{
		mSpecularColor = color;
	}
	void LightClass::SetSpecularPower(float power)
	{
		mSpecularPower = power;
	}
	void LightClass::SetAmbientColor(XMFLOAT4 color)
	{
		mAmbient = color;
	}
	void LightClass::SetDiffuseColor(XMFLOAT4 color) {
		mDiffuseColor = color;
	}
	void LightClass::SetDirection(XMFLOAT3 direction) {
		mDirection = direction;
	}
}