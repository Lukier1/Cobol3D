#include "TextClass.h"
#include "Logger.h"
#include "MyMath.h"

#include <Windows.h>
#include <string>

namespace Cobol {
	TextClass::TextClass() {
		mFontShader = 0;
		mLastTime = 0;
	}
	TextClass::TextClass(const TextClass&) 
	{

	}
	TextClass::~TextClass() {

	}
	bool TextClass::Initialize(ID3D11Device* dev, ID3D11DeviceContext * devCon, HWND hwnd, int screenWidth, int screenHeight)
	{
		bool result;
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;

		mFont = new FontClass();
		if(!mFont)
		{
			return false;
		}
		result = mFont->Init(dev,"data/fontIndex.txt", "data/font.gif");
		if(!result)
		{
			LOG_MSGERR("Problem with init font");	
			return false;
		}
		mFontShader = new FontShaderClass();
		mFontShader->Init(dev,hwnd);
		result = InitializeSentence(&mSentence1,40, dev);
		if(!result)
		{
			LOG_MSGERR("Problem with init sentence");	
			return false;
		}
		result = UpdateSentence(mSentence1,"Hello World!", 100,100 , 1.0f, 1.0f, 1.0f, devCon);
		if(!result)
		{
			return false;
		}
		result = InitializeSentence(&mSentence2,16, dev);
		if(!result)
		{
			LOG_MSGERR("Problem with init sentence");	
			return false;
		}
		result = UpdateSentence(mSentence2,"Goodbye", 100,200 , 1.0f, 1.0f, 0.0f, devCon);
		if(!result)
		{
			return false;
		}
		return true;
	}
	void TextClass::Shutdown() {
		ReleaseSentence(&mSentence1);
		ReleaseSentence(&mSentence2);
		if(mFontShader)
		{
			delete mFontShader;
			mFontShader = 0;
		}
		if(mFont)
		{
			mFont->Shutdown();
			delete mFont;
			mFont = 0 ;
		}
	}
	bool TextClass::Render(ID3D11DeviceContext * devCon)
	{
		//FPS Coun
		std::string wyjscie = "FPS: ";
		long long int freq;
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		float ticksPerMs = (float)(freq / 1000);
		long long int currTime;
		
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		float diffTime = (currTime - mLastTime)/ticksPerMs ;
		mLastTime = currTime;


		int fps = (diffTime!=0) ?  (int)(1000/diffTime) : -1;
		wyjscie+=toString(fps);
		UpdateText2(wyjscie.c_str(),devCon);
		////////////////////////////////

		bool result;

		result = RenderSentence(devCon, mSentence1);
		if(!result)
			return false;
		result = RenderSentence(devCon, mSentence2);
		return result;
	}

	bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
	{
		VertexType * vertices;
		unsigned long * indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result; 
		int i;

		*sentence = new SentenceType;
		if(!sentence)
		{
			return false;
		}
		(*sentence)->vertexBuffer = 0;
		(*sentence)->indexBuffer  = 0;
		(*sentence)->maxLength = maxLength;
		(*sentence)->vertexCount = 6*maxLength;

		(*sentence)->indexCount = (*sentence)->vertexCount;
		vertices = new VertexType[(*sentence)->vertexCount];
		if(!vertices)
		{
			return false;
		}
		indices = new unsigned long[(*sentence)->indexCount];
		if(!indices)
			return false;
		memset(vertices, 0, (sizeof(VertexType)*(*sentence)->vertexCount));
		for(int i = 0; i < (*sentence)->indexCount; ++i)
		{
			indices[i] = i;
		}
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
		if(FAILED(result))
		{
			return false;
		}
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Release the vertex array as it is no longer needed.
		delete [] vertices;
		vertices = 0;

		// Release the index array as it is no longer needed.
		delete [] indices;
		indices = 0;

		return true;
	}
	bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
			    ID3D11DeviceContext* devCon)
	{
		int numLetters;
		VertexType* vertices;
		float drawX, drawY;
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		VertexType* verticesPtr;

		sentence->red = red;
		sentence->blue = blue;
		sentence->green = green;

		numLetters = (int)strlen(text);

		if(numLetters > sentence->maxLength)
		{
			return false;
		}
		vertices = new VertexType[sentence->vertexCount];
		if(!vertices)
		{
			return false;
		}
		memset(vertices,0,sizeof(VertexType)*sentence->vertexCount);
		
		drawX = (float)(((mScreenWidth / 2) * -1) + positionX);
		drawY = (float)((mScreenHeight / 2) - positionY);

		mFont->BuildVertexArray((void*)vertices,text,drawX,drawY);

		result = devCon->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD,0, &mappedResource);
		if(FAILED(result))
		{
			return false;
		}
		verticesPtr = (VertexType*)mappedResource.pData;
		memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*sentence->vertexCount));
		devCon->Unmap(sentence->vertexBuffer, 0);
		delete[] vertices;

		return true;
	}
	void TextClass::ReleaseSentence(SentenceType** sentence)
	{
		if(*sentence)
		{
			// Release the sentence vertex buffer.
			if((*sentence)->vertexBuffer)
			{
				(*sentence)->vertexBuffer->Release();
				(*sentence)->vertexBuffer = 0;
			}

			// Release the sentence index buffer.
			if((*sentence)->indexBuffer)
			{
				(*sentence)->indexBuffer->Release();
				(*sentence)->indexBuffer = 0;
			}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
		}
	}
	bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence) {
		
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	mFontShader->Render(deviceContext, sentence->indexCount,mFont->GetTexture(), 
				      pixelColor);
	

	return true;
	}
	void TextClass::UpdateText(const char * text,ID3D11DeviceContext * devCon)
	{
		char * newText = new char[strlen(text)+2];
		strcpy(newText,text);
		UpdateSentence(mSentence1, newText, 100,100 , 1.0f, 1.0f, 1.0f, devCon);
		delete[] newText;
	}
	void TextClass::UpdateText2(const char * text,ID3D11DeviceContext * devCon)
	{
		char * newText = new char[strlen(text)+2];
		strcpy(newText,text);
		UpdateSentence(mSentence2, newText, 10, 10 , 0.0f, 1.0f, 0.0f, devCon);
		delete[] newText;
	}
}
