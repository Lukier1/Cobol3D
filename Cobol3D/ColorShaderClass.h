#ifndef __COLORSHADERCLASS_H__
#define __COLORSHADERCLASS_H__
#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )

namespace Cobol {
	class ColorShaderClass {
	private:
		// Moze wstawiac tutaj bufory
	public:
		ColorShaderClass();
		ColorShaderClass(const ColorShaderClass&);
		~ColorShaderClass();

		bool Init(ID3D11Device*, HWND);
		void Shutdown();
		void Render(ID3D11DeviceContext*,int);
	private:
		bool InitShader(ID3D11Device*, HWND, CHAR*, CHAR*);
		void ShutdownShader();
		void OutputShaderErrorMessage(ID3D10Blob*, HWND, CHAR*);

		bool SetShaderParameters(ID3D11DeviceContext*);
		void RenderShader(ID3D11DeviceContext*, int);
	
		ID3D11VertexShader* mVertexShader;
		ID3D11PixelShader* mPixelShader;
		ID3D11InputLayout* mLayout;
	};
}
#endif