#include "TextureClass.h"

namespace Cobol {
	TextureClass::TextureClass() {
		mTexture = 0;
		mFilename = "EMPTY";
	}
	TextureClass::TextureClass(const TextureClass& tex) {

	}
	TextureClass::~TextureClass() {
		Shutdown();
	}
	bool TextureClass::Init(ID3D11Device* dev, char * filename) {
		HRESULT result;
		result = D3DX11CreateShaderResourceViewFromFile(dev,filename,NULL,NULL,&mTexture,NULL);
		mFilename = filename;
		return !(FAILED(result));
	}
	void TextureClass::Shutdown() {
		if(mTexture)
		{
			mTexture->Release();
			mTexture = 0;
		}
	}
};