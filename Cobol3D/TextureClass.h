#ifndef __TEXTURECLASS_H__
#define __TEXTURECLASS_H__

#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#include <string>
#pragma warning ( default : 4005 )
namespace Cobol {
	class TextureClass {
	private:
		ID3D11ShaderResourceView* mTexture;
		std::string mFilename;
	public:
		TextureClass();
		TextureClass(const TextureClass&);
		~TextureClass();
	
		bool Init(ID3D11Device*, char*);
		void Shutdown();

		ID3D11ShaderResourceView* getTexture() { return mTexture; }
	};
}
#endif