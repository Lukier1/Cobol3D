#ifndef __ROOT_H__
#define __ROOT_H__

#include <Windows.h>

#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )
class Mesh; class Material; class SceneMesh;
namespace Cobol {
	class Logger;
	
	class Root {
	private:
		struct LightBuffer {
			XMFLOAT4 dir[2];
			XMFLOAT4 color[2];
		};
		struct ConstantBuffer {
			XMMATRIX worldMatrix;
			XMMATRIX viewMatrix;
			XMMATRIX projMatrix;
			XMFLOAT4 vlightDir[2];
			XMFLOAT4 vlightColor[2];
		
		};
		Mesh * mesh; Material * material; SceneMesh * sMesh;
		
		unsigned short mResW, mResH;
		static LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

		D3D_DRIVER_TYPE mDriverType;
		D3D_FEATURE_LEVEL mFeatureLevel;
		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DContext;
		IDXGISwapChain* mSwapChain;
		ID3D11RenderTargetView* mBackBufferTarget;

		ID3D11Texture2D* mDepthTexture;
		ID3D11DepthStencilView* mDepthStencilView;
		HWND mHwnd;

		ID3D11Buffer* mViewCB, * mProjCB, *mWorldCB, * mCB;
		XMMATRIX mViewMatrix, mProjMatrix;
		bool InitDirectX();
		bool run;

		void Render();
		void InitBuffers();
		void Clean();
		void DrawSceneMesh(SceneMesh * scenMesh);
		Logger  logg;
	public:
		bool NewWindow(HINSTANCE &hInstance, int cmdShow);
		bool Run();
		
		Root() : logg("moj_log.log") { mResW=800; mResH=600; }
		~Root() { Clean(); }
	};
}

#endif