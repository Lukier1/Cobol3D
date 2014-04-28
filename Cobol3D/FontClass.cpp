#include "FontClass.h"
#include <fstream>
namespace Cobol {
	FontClass::FontClass() {
		mFont = nullptr;
		mTexture = nullptr;
	}
	FontClass::FontClass(const FontClass&)
	{

	}
	FontClass::~FontClass() {

	}
	bool FontClass::Init(ID3D11Device * device, char * filename, char* bitmapName)
	{
		bool result;

		result = LoadFontData(filename);
		if(!result)
			return false;

		result = LoadTexture(device, bitmapName);
		if(!result)
			return false;


		return result;
	}
	void FontClass::Shutdown() {
		ReleaseFontData();
		ReleaseTexture();
	}

	ID3D11ShaderResourceView * FontClass::GetTexture() {
		return mTexture->getTexture();
	}

	void FontClass::BuildVertexArray(void*vertices , char*sentence ,float drawX, float drawY) {
		VertexType * vertexPtr;
		int numLetters, index, letter;

		vertexPtr = (VertexType*)vertices;
		numLetters = (int)strlen(sentence);
		index=0;
		for(int i = 0; i < numLetters; i++)
		{
			letter=(int)(sentence[i])-32;
			if(letter==0)
			{
				drawX+=3.0f;
			}
			else
			{
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + mFont[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + mFont[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + mFont[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(mFont[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + mFont[letter].size + 1.0f;
			}
		}

	}
	bool FontClass::LoadFontData(char * filename)
	{
		std::ifstream in;
		int i;
		char temp;

		mFont = new FontType[95];
		if(!mFont)
			return false;

		in.open(filename);
		if(in.fail())
		{
			return false;
		}
		for(i=0; i < 95; ++i)
		{
			in.get(temp);
			while(temp != ' ')
				in.get(temp);
			in.get(temp);
			while(temp != ' ')
				in.get(temp);
			in >> mFont[i].left; mFont[i].left*=1.712;
			in >> mFont[i].right; mFont[i].right*=1.712;
			in >> mFont[i].size;
		}
		in.close();
		return true;
	}
	void FontClass::ReleaseFontData() {
		delete mFont;
		mFont = nullptr;
	}
	bool FontClass::LoadTexture(ID3D11Device* dev, char* filename)
	{
		mTexture = new TextureClass();
		return mTexture->Init(dev,filename);
	}
	void FontClass::ReleaseTexture() {
		if(mTexture) {
			mTexture->Shutdown();
			delete mTexture;
			mTexture = 0;
		}
	}
};