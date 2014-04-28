#ifndef __FONTCLASS_H__
#define __FONTCLASS_H__

#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )

#include "TextureClass.h"
namespace Cobol {
	class FontClass {
	private:
		struct FontType {
			float left,right;
			float size;
		};
		struct VertexType {
			XMFLOAT3 position;
			XMFLOAT2 texture;
		};

	public:
		FontClass();
		FontClass(const FontClass&);
		~FontClass();
	
		bool Init(ID3D11Device* device,char* filename,char* bitmapName);
		void Shutdown();

		ID3D11ShaderResourceView * GetTexture();

		void BuildVertexArray(void*, char*,float,float);

		
	private:
		bool LoadFontData(char*);
		void ReleaseFontData();
		bool LoadTexture(ID3D11Device*,char*);
		void ReleaseTexture();

	private:
		FontType* mFont;
		TextureClass* mTexture;
	};
}
#endif