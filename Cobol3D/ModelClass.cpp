#include "ModelClass.h"
#include <cstdio>
#include "Logger.h"

namespace Cobol {
	ModelClass::ModelClass() {
		mVertexBuffer = nullptr;
		mIndexBuffer = nullptr;
		mTexture = nullptr;
	}
	ModelClass::ModelClass(const ModelClass& other)
	{
	}
	ModelClass::~ModelClass() 
	{
	}
	
	bool ModelClass::Init(ID3D11Device* device,char* filename, WCHAR* modelName)
	{
		LoadModel(modelName);
		bool res = InitBuffers(device);
		
		if(!res) {
			LOG_MSGERR("Problem with init buffers in ModelClass");
		}
		LoadTexture(device,filename);
		
		return res;
	}
	void ModelClass::Shutdown() {
		ShutdownBuffers();
		ReleaseModel();
		mTexture->Shutdown();
		delete mTexture;
	}
	void ModelClass::Render(ID3D11DeviceContext* devCon)
	{
		RenderBuffer(devCon);
	}
	int ModelClass::GetIndexCount() {
		return mIndexCount;
	}

	ID3D11ShaderResourceView * ModelClass::getTexture() {
		return mTexture->getTexture();
	}

	void ModelClass::ShutdownBuffers() {
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
	bool ModelClass::InitBuffers(ID3D11Device* device)
	{
		VertexType* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;
		
		//mVertexCount =3;
		//mIndexCount = 3;

		vertices = new VertexType[mVertexCount];
		if(!vertices)
			return false;

		indices = new unsigned long[mIndexCount];
		if(!indices)
			return false;
		/*vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
		vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
		vertices[0].normal = XMFLOAT3(0.0f,0.0f,-1.0f);

		vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[1].texture = XMFLOAT2(0.5f, 0.0f);
		vertices[1].normal = XMFLOAT3(0.0f,0.0f,-1.0f);

		vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
		vertices[2].normal = XMFLOAT3(0.0f,0.0f,-1.0f);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		*/
		// Load the vertex array and index array with data.
		for(int i=0; i<mVertexCount; i++)
		{
			vertices[i].position = XMFLOAT3(mModel[i].x, mModel[i].y, mModel[i].z);
			vertices[i].texture =  XMFLOAT2(mModel[i].tu, mModel[i].tv);
			vertices[i].normal =  XMFLOAT3(mModel[i].nx, mModel[i].ny, mModel[i].nz);

			indices[i] = i;
		}
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType)*mVertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0; 
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);

		memcpy(&indexBufferDesc, &vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		indexBufferDesc.ByteWidth = sizeof(unsigned long)*mIndexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; 

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
	void ModelClass::RenderBuffer(ID3D11DeviceContext* devCon)
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

	bool ModelClass::LoadModel(WCHAR* filename)
	{
		std::ifstream fin;
		char input;
		int i;

		fin.open(filename);
		if(fin.fail())
			return false;
		fin.get(input);
		while(input!=':')
		{
			fin.get(input);
		}
		fin >> mVertexCount;
		mIndexCount = mVertexCount;

		mModel = new ModelType[mVertexCount];
		if(!mModel)
			return false;
		fin.get(input);
		while(input!=':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);
		for(int i =0; i < mVertexCount; i++)
		{
			fin >> mModel[i].x >> mModel[i].y >> mModel[i].z;
			fin >> mModel[i].tu >> mModel[i].tv;
			fin >> mModel[i].nx >> mModel[i].ny >> mModel[i].nz;
		}
		fin.close();
	}

	void ModelClass::ReleaseModel() {

	}
	bool ModelClass::LoadTexture(ID3D11Device * dev, char * filename) {
		mTexture = new TextureClass();
		return mTexture->Init(dev,filename);
	}
	void ModelClass::ReleaseTexture() {
		if(mTexture) {
			delete mTexture;
			mTexture = 0;
		}
	}
}
