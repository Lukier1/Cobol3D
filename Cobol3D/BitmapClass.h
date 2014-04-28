#ifndef __BITMAPCLASS_H__
#define __BITMAPCLASS_H__

#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )

#include "TextureClass.h"

#include <fstream>

namespace Cobol {
	class BitmapClass {
	private:
		struct VertexType {
			XMFLOAT3 position;
			XMFLOAT2 texture;
		};

		

	public:
		BitmapClass();
		BitmapClass(const BitmapClass&);
		~BitmapClass();		

		bool Init(ID3D11Device* device,char* filename, WCHAR* modelName, XMFLOAT2 screen,XMFLOAT2 bitmap);
		void Shutdown();
		void Render(ID3D11DeviceContext*,int posX, int posY);

		int GetIndexCount();
		ID3D11ShaderResourceView * getTexture();

	private:
		void ShutdownBuffers();
		bool InitBuffers(ID3D11Device*);
		void RenderBuffer(ID3D11DeviceContext*);
		bool UpdateBuffer(ID3D11DeviceContext*, XMFLOAT2);
		bool LoadTexture(ID3D11Device*, char*);
		void ReleaseTexture();

	
	private:
		ID3D11Buffer * mVertexBuffer, *mIndexBuffer;
		int mVertexCount, mIndexCount;

		TextureClass * mTexture;

		XMFLOAT2 mScreen,mBitmap,mPreviousPos;
		
	};
}
#endif