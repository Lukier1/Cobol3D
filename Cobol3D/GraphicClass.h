#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__
#pragma warning ( disable : 4005 )
	#include<dxgi.h>
	#include<d3dx11.h>
	#include<DxErr.h>
	#include <xnamath.h>


#pragma warning ( default : 4005 )

#include <ctime>

#include "Device.h"
#include "Camera.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"
#include "LightClass.h"
#include "LightShaderClass.h"
#include "BitmapClass.h"
#include "TextClass.h"

namespace Cobol {
	struct InputData {
		int MouseX, MouseY;
		int IX, IY;
		float appTime;
		char litera;
	};
	class GraphicClass {
	private:
		struct VSPerFrameCB { // register 0
			
			XMMATRIX projMatrix;
			float appTime;
		};
		struct VSPerSkinedCB {// register 1
			XMMATRIX bone;
		};
		struct VSPerStaticCB { // register 2
			XMMATRIX worldMatrix;
		};
		struct VSPerPassCB {// register 3
			XMMATRIX viewMatrix;
			XMFLOAT3 cameraPosition;
			float padding;
		};
		struct VSPerMaterialCB {// register 4
			XMFLOAT4 BDRFCoefficients;
		};

		
		ID3D11Buffer * mFrameCB, * mSkinCB, * mStatCB, * mPassCB, * mMaterCB;

		Device * mDevice;
		Camera * mCamera;
		ModelClass * mModel, *mModel2;
		TextClass * mText;
		TextureShaderClass * mTextureShader;
		LightShaderClass * mLightShader;
		LightClass * mLight;
		BitmapClass * mBitmap;
		void PrepareBuffers();
		int InitBuffer();
		const clock_t beginTime;
	public:
		

		GraphicClass();
		~GraphicClass();
		bool Init(HWND &hwnd, int screenH, int screenW);
		void Frame(InputData &input);
		int Clean();
	};
}

#endif