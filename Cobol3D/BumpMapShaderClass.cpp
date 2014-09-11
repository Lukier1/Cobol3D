#include "BumpMapShaderClass.h"

#include "Logger.h"

namespace Cobol {
	BumpMapShaderClass::BumpMapShaderClass() {
		mVertexShader = nullptr;
		mPixelShader = nullptr;
		mLayout = nullptr;
		mSampleState = nullptr;
		mLightBuffer = nullptr;
	}

	BumpMapShaderClass::BumpMapShaderClass(const BumpMapShaderClass& other) {

	}
	BumpMapShaderClass::~BumpMapShaderClass() {

	}

	bool BumpMapShaderClass::Init(ID3D11Device * device, HWND hwnd) {
		bool result = InitShader(device, hwnd, "BumpShader.hlsl", "BumpShader.hlsl");
		return result;
	}
	void BumpMapShaderClass::Shutdown() {
		ShutdownShader();
	}
	void BumpMapShaderClass::Render(ID3D11DeviceContext * devCon, int indexCount, ID3D11ShaderResourceView ** textures,
		XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT4 specularColor, float specularPower)
	{
		SetShaderParameters(devCon, textures, lightDirection, diffuseColor, ambientColor, specularColor, specularPower);

		RenderShader(devCon, indexCount);
	}
	bool BumpMapShaderClass::InitShader(ID3D11Device* device, HWND hwnd, CHAR* vsFilename, CHAR* psFilename)
	{
		HRESULT result;
		ID3D10Blob * errorMessage = 0, *vertexShaderBuffer = 0, *pixelShaderBuffer = 0;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
		unsigned int numElements;
		D3D11_SAMPLER_DESC samplerDesc;
		//D3D11_BUFFER_DESC matrixBufferDesc;

		result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "BumpMapVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
		if (FAILED(result))
		{
			if (errorMessage)
			{

				OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);

			}
			else
			{
				MessageBox(hwnd, vsFilename, "Missing Shader File", MB_OK);
			}
			return false;
		}
		result = D3DX11CompileFromFile(psFilename, NULL, NULL, "BumpMapPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
		if (FAILED(result))
		{
			if (errorMessage)
			{


				OutputShaderErrorMessage(errorMessage, hwnd, psFilename);

			}
			else
			{
				MessageBox(hwnd, vsFilename, "Missing Shader File", MB_OK);
			}
			return false;
		}
		result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
		if (FAILED(result))
			return false;
		result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);
		if (FAILED(result))
			return false;

		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "TEXCOORD";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		polygonLayout[2].SemanticName = "NORMAL";
		polygonLayout[2].SemanticIndex = 0;
		polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[2].InputSlot = 0;
		polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[2].InstanceDataStepRate = 0;

		polygonLayout[3].SemanticName = "TANGENT";
		polygonLayout[3].SemanticIndex = 0;
		polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[3].InputSlot = 0;
		polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[3].InstanceDataStepRate = 0;

		polygonLayout[4].SemanticName = "BINORMAL";
		polygonLayout[4].SemanticIndex = 0;
		polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[4].InputSlot = 0;
		polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[4].InstanceDataStepRate = 0;

		numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
		result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
			vertexShaderBuffer->GetBufferSize(), &mLayout);


		vertexShaderBuffer->Release();
		vertexShaderBuffer = 0;

		pixelShaderBuffer->Release();
		pixelShaderBuffer = 0;

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		result = device->CreateSamplerState(&samplerDesc, &mSampleState);
		if (FAILED(result))
			return false;

		D3D11_BUFFER_DESC descLight;
		ZeroMemory(&descLight, sizeof(descLight));
		descLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		descLight.ByteWidth = sizeof(LightCB);
		descLight.Usage = D3D11_USAGE_DYNAMIC;
		descLight.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		result = device->CreateBuffer(&descLight, NULL, &mLightBuffer);
		if (FAILED(result))
			return false;

		return true;
	}
	void BumpMapShaderClass::ShutdownShader() {
		if (mLightBuffer)
		{
			mLightBuffer->Release();
			mLightBuffer = 0;
		}
		if (mSampleState)
		{
			mSampleState->Release();
			mSampleState = 0;
		}
		if (mVertexShader)
		{
			mVertexShader->Release();
			mVertexShader = 0;
		}
		if (mPixelShader)
		{
			mPixelShader->Release();
			mPixelShader = 0;
		}
		if (mLayout) {
			mLayout->Release();
			mLayout = 0;
		}
	}
	void BumpMapShaderClass::OutputShaderErrorMessage(ID3D10Blob* blob, HWND hwnd, CHAR* shaderFilename) {
		char * compileError = (char*)blob->GetBufferPointer();
		unsigned long bufferSize = blob->GetBufferSize();
		std::string error("");
		for (unsigned int i = 0; i < bufferSize; i++)
			error += compileError[i];
		Logger::GetSingletonPtr()->WriteError(error);
		blob->Release();

		MessageBox(hwnd, "Error compiling shader : check log file to details", shaderFilename, MB_ICONERROR | MB_OK);

	}
	bool BumpMapShaderClass::SetShaderParameters(ID3D11DeviceContext * devCon, ID3D11ShaderResourceView ** textures,
		XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT4 specularColor, float specularPower) {
		devCon->PSSetShaderResources(0, 3, textures);
		LightCB * dataPtr;
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		devCon->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		// Get a pointer to the data in the constant buffer.
		dataPtr = (LightCB*)mappedResource.pData;
		// Copy the matrices into the constant buffer.
		dataPtr->diffuseColor = diffuseColor;
		dataPtr->lightDirection = lightDirection;
		dataPtr->ambientColor = ambientColor;
		dataPtr->specularPower = specularPower;
		dataPtr->specularColor = specularColor;
		// Unlock the constant buffer.
		devCon->Unmap(mLightBuffer, 0);
		devCon->PSSetConstantBuffers(5, 1, &mLightBuffer);
		return true;
	}
	void BumpMapShaderClass::RenderShader(ID3D11DeviceContext * devCon, int indexCount) {


		devCon->IASetInputLayout(mLayout);

		devCon->VSSetShader(mVertexShader, NULL, 0);
		devCon->PSSetShader(mPixelShader, NULL, 0);

		devCon->PSSetSamplers(0, 1, &mSampleState);
		devCon->DrawIndexed(indexCount, 0, 0);
	}
}