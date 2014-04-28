#ifndef __LIGHTCLASS_H__
#define __LIGHTCLASS_H__

#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )

namespace Cobol {
	class LightClass {
	public:
		LightClass();
		LightClass(const LightClass&);
		~LightClass();

		void SetDiffuseColor(XMFLOAT4);
		void SetDirection(XMFLOAT3);
		void SetAmbientColor(XMFLOAT4);
		void SetSpecularColor(XMFLOAT4);
		void SetSpecularPower(float);
		XMFLOAT4 GetDiffuseColor() { return mDiffuseColor; }
		XMFLOAT3 GetDirection() { return mDirection; }
		XMFLOAT4 GettAmbientColor() { return mAmbient; }
		XMFLOAT4 GetSpecularColor() { return mSpecularColor; }
		float GetSpecularPower() { return mSpecularPower; }
	private:
		XMFLOAT4 mSpecularColor;
		float mSpecularPower;
		XMFLOAT4 mDiffuseColor;
		XMFLOAT3 mDirection;
		XMFLOAT4 mAmbient;
	};
};
#endif