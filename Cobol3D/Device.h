#ifndef __DEVICE_H__
#define __DEVICE_H__

#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )

namespace Cobol {
	enum RESULT {
		R_FAILED = 0,
		R_OK = 1
	};
	class Device {
	private:
		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DContext;
		D3D_DRIVER_TYPE mDriverType;
		D3D_FEATURE_LEVEL mFeatureLevel;
		IDXGISwapChain* mSwapChain;
		ID3D11RenderTargetView* mBackBufferTarget;
		ID3D11RasterizerState * mRasterState;
		ID3D11BlendState * mBlendState;

		ID3D11Texture2D* mDepthTexture;
		ID3D11DepthStencilView* mDepthStencilView;
		ID3D11DepthStencilState* mDepthStencilState;
		ID3D11DepthStencilState* mDepthDisabledStencilState;
		XMMATRIX mViewMatrix, mProjMatrix;
		XMMATRIX mOrthoMatrix;

		
	public:
		bool mVsync;

		RESULT InitDevice(int screenW, int screenH, HWND * hwnd);
		void BeginScene(float red, float green, float blue, float alpha);
		void EndScene();
		ID3D11Device * getDevice() { return mD3DDevice; }
		ID3D11DeviceContext * getContext() { return mD3DContext; }

		
		void GetProjectionMatrix(XMMATRIX& m) { m = mProjMatrix; }
		void GetOrthoMatrix(XMMATRIX& m) { m = mOrthoMatrix; }

		void SetBufferZ(bool active);
		Device();
		~Device(); 
	};
};
#endif