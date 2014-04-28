#include "BitmapClass.h"
#include <cstdio>
#include "Logger.h"

namespace Cobol {
	BitmapClass::BitmapClass() {
		mVertexBuffer = nullptr;
		mIndexBuffer = nullptr;
		mTexture = nullptr;
	}
	BitmapClass::BitmapClass(const BitmapClass& other)
	{
	}
	BitmapClass::~BitmapClass() 
	{
	}
	
	bool BitmapClass::Init(ID3D11Device* device,char* filename, WCHAR* modelName, XMFLOAT2 screen,XMFLOAT2 bitmap)
	{
		mScreen=screen;
		mBitmap=bitmap;
		mPreviousPos=XMFLOAT2(-1.0f,-1.0f);
		bool res = InitBuffers(device);
		
		if(!res) {
			LOG_MSGERR("Problem with init buffers in BitmapClass");
		}
		res = LoadTexture(device,filename);
		
		return res;
	}
	void BitmapClass::Shutdown() {
		ShutdownBuffers();
		
		mTexture->Shutdown();
		delete mTexture;
	}
	void BitmapClass::Render(ID3D11DeviceContext* devCon,int posX, int posY)
	{
		
		bool result = UpdateBuffer(devCon, XMFLOAT2(posX, posY));
		if(!result)
		{
			return;
		}
		RenderBuffer(devCon);
	}
	int BitmapClass::GetIndexCount() {
		return mIndexCount;
	}

	ID3D11ShaderResourceView * BitmapClass::getTexture() {
		return mTexture->getTexture();
	}

	void BitmapClass::ShutdownBuffers() {
		if(mIndexBuffer)
		{
			mIndexBuffer->Release();
			mIndexBuffer = nullptr;
		}

		if(mVertexBuffer)
		{
			mVertexBuffer->Release();
			mVertexBuffer = nullptr;
		}
	}
	bool BitmapClass::InitBuffers(ID3D11Device* device)
	{
		VertexType* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;
		
		mVertexCount =6;
		mIndexCount = mVertexCount;
		
		vertices = new VertexType[mVertexCount];
		
		if(!vertices)
			return false;

		indices = new unsigned long[mIndexCount];
		if(!indices)
			return false;

		memset(vertices,0,(sizeof(VertexType))*mVertexCount);

		// Load the index array with data.
		for(int i=0; i<mIndexCount; i++)
		{
			indices[i] = i;
		}
		
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VertexType)*mVertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);

		//memcpy(&indexBufferDesc, &vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&indexBufferDesc,sizeof(D3D11_BUFFER_DESC));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long)*mIndexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; 
		indexBufferDesc.CPUAccessFlags = 0;
		

		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
		if(FAILED(result))
		{
				return false;
		}

		delete[] vertices;
		vertices = 0;

		delete[] indices;
		indices = 0;

		return true;
	}
	bool BitmapClass::UpdateBuffer(ID3D11DeviceContext * devCon, XMFLOAT2 position)
	{
		float right, left, top, bottom;
		VertexType * verticesPtr=0, *vertices = 0;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT res;
		
		if(position.x == mPreviousPos.x  && position.y == mPreviousPos.y  )
		{
			return true;
		}
		mPreviousPos = position;
		left = (float)(mScreen.x/2*(-1))+(float)position.x;
		right = left + (float)mBitmap.x;
		top =  (float)(mScreen.y/2)-(float)position.y;
		bottom = top -  (float)mBitmap.y;

		vertices = new VertexType[mVertexCount];
		vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
		vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
		vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

		vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
		vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

		// Second triangle.
		vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
		vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
		vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

		vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
		vertices[5].texture = XMFLOAT2(1.0f, 1.0f);
	
		res = devCon->Map(mVertexBuffer,0,D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if(FAILED(res)) {
			LOG_MSGERR("Problem with mapping vertices in BitmapClass");
			return false;
		}
		verticesPtr = (VertexType*)mappedResource.pData;
		memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*mVertexCount));
		devCon->Unmap(mVertexBuffer,0);
		delete[] vertices;
		return true;
	}

	void BitmapClass::RenderBuffer(ID3D11DeviceContext* devCon)
	{
		unsigned int stride;
		unsigned int offset;
		stride = sizeof(VertexType); 
		offset = 0;
    
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		devCon->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		devCon->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;

	}

	

	bool BitmapClass::LoadTexture(ID3D11Device * dev, char * filename) {
		mTexture = new TextureClass();
		return mTexture->Init(dev,filename);
	}
	void BitmapClass::ReleaseTexture() {
		if(mTexture) {
			delete mTexture;
			mTexture = 0;
		}
	}
}
