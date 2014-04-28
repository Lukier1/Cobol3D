#include "TextureShaderClass.h"

#include "Logger.h"

namespace Cobol {
	TextureShaderClass::TextureShaderClass() {
		mVertexShader = nullptr;
		mPixelShader = nullptr;
		mLayout = nullptr;
	}

	TextureShaderClass::TextureShaderClass(const TextureShaderClass& other) {
		
	}
	TextureShaderClass::~TextureShaderClass() {

	}

	bool TextureShaderClass::Init(ID3D11Device * device, HWND hwnd) {
		bool result = InitShader(device, hwnd, "texture.hlsl", "texture.hlsl");
		return result;
	}
	void TextureShaderClass::Shutdown() {
		ShutdownShader();
	}
	void TextureShaderClass::Render(ID3D11DeviceContext * devCon, int indexCount,ID3D11ShaderResourceView * texture)
	{
		SetShaderParameters(devCon,texture);

		RenderShader(devCon,indexCount);
	}
	bool TextureShaderClass::InitShader(ID3D11Device* device, HWND hwnd, CHAR* vsFilename, CHAR* psFilename)
	{
		HRESULT result;
		ID3D10Blob * errorMessage = 0, * vertexShaderBuffer = 0, * pixelShaderBuffer = 0;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
		unsigned int numElements;
		D3D11_SAMPLER_DESC samplerDesc;
		//D3D11_BUFFER_DESC matrixBufferDesc;

		result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "TextureVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
		if(FAILED(result))
		{
			if(errorMessage)
			{
				   
					OutputShaderErrorMessage(errorMessage, hwnd,  vsFilename);
				
			}
			else
			{
				MessageBox(hwnd, vsFilename, "Missing Shader File", MB_OK);
			}
			return false;
		}
		result = D3DX11CompileFromFile(psFilename, NULL, NULL, "TexturePS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
		if(FAILED(result))
		{
			if(errorMessage)
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
		if(FAILED(result))
			return false;
		result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);
		if(FAILED(result))
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

		numElements = sizeof(polygonLayout)/sizeof(polygonLayout[0]);
		result = device->CreateInputLayout(polygonLayout,numElements, vertexShaderBuffer->GetBufferPointer(), 
					   vertexShaderBuffer->GetBufferSize(), &mLayout);
		
		
		vertexShaderBuffer->Release();
		vertexShaderBuffer = 0;

		pixelShaderBuffer->Release();
		pixelShaderBuffer = 0;

		ZeroMemory(&samplerDesc,sizeof(samplerDesc));

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU  = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV  = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW  = D3D11_TEXTURE_ADDRESS_WRAP;
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
		if(FAILED(result))
			return false;
		return true;
	}
	void TextureShaderClass::ShutdownShader() {
		if(mSampleState)
		{
			mSampleState->Release();
			mSampleState = 0;
		}
		if(mVertexShader)
		{
			mVertexShader->Release();
			mVertexShader = 0;
		}
		if(mPixelShader)
		{
			mPixelShader->Release();
			mPixelShader = 0;
		}
		if(mLayout) {
			mLayout->Release();
			mLayout = 0;
		}
	}
	void TextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* blob, HWND hwnd, CHAR* shaderFilename) {
		char * compileError = (char*)blob->GetBufferPointer();
		unsigned long bufferSize = blob->GetBufferSize();
		std::string error("");
		for(unsigned int i =0 ; i < bufferSize; i++)
			error+=compileError[i];
		Logger::GetSingletonPtr()->WriteError(error);
		blob->Release();

		MessageBox(hwnd, "Error compiling shader : check log file to details", shaderFilename, MB_ICONERROR | MB_OK);

	}
	bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext * devCon,ID3D11ShaderResourceView * texture) {
		devCon->PSSetShaderResources(0, 1, &texture);
		return true;
	}
	void TextureShaderClass::RenderShader(ID3D11DeviceContext * devCon, int indexCount) {
		

		devCon->IASetInputLayout(mLayout);

		devCon->VSSetShader(mVertexShader, NULL, 0);
		devCon->PSSetShader(mPixelShader, NULL, 0);
		
		devCon->PSSetSamplers(0,1,&mSampleState);
		devCon->DrawIndexed(indexCount, 0,0);
	}
}