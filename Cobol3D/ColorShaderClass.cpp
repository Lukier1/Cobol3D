#include "ColorShaderClass.h"

#include "Logger.h"

namespace Cobol {
	ColorShaderClass::ColorShaderClass() {
		mVertexShader = nullptr;
		mPixelShader = nullptr;
		mLayout = nullptr;
	}

	ColorShaderClass::ColorShaderClass(const ColorShaderClass& other) {
		
	}
	ColorShaderClass::~ColorShaderClass() {

	}

	bool ColorShaderClass::Init(ID3D11Device * device, HWND hwnd) {
		bool result = InitShader(device, hwnd, "shaders.hlsl", "shaders.hlsl");
		return result;
	}
	void ColorShaderClass::Shutdown() {
		ShutdownShader();
	}
	void ColorShaderClass::Render(ID3D11DeviceContext * devCon, int indexCount)
	{
		SetShaderParameters(devCon);

		RenderShader(devCon,indexCount);
	}
	bool ColorShaderClass::InitShader(ID3D11Device* device, HWND hwnd, CHAR* vsFilename, CHAR* psFilename)
	{
		HRESULT result;
		ID3D10Blob * errorMessage = 0, * vertexShaderBuffer = 0, * pixelShaderBuffer = 0;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
		unsigned int numElements;
		//D3D11_BUFFER_DESC matrixBufferDesc;

		result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "ColorVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
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
		result = D3DX11CompileFromFile(psFilename, NULL, NULL, "ColorPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
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
	
		polygonLayout[1].SemanticName = "COLOR";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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

		return true;
	}
	void ColorShaderClass::ShutdownShader() {
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
	void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* blob, HWND hwnd, CHAR* shaderFilename) {
		char * compileError = (char*)blob->GetBufferPointer();
		unsigned long bufferSize = blob->GetBufferSize();
		std::string error("");
		for(unsigned int i =0 ; i < bufferSize; i++)
			error+=compileError[i];
		Logger::GetSingletonPtr()->WriteError(error);
		blob->Release();

		MessageBox(hwnd, "Error compiling shader : check log file to details", shaderFilename, MB_ICONERROR | MB_OK);

	}
	bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext * devCon) {
		return true;
	}
	void ColorShaderClass::RenderShader(ID3D11DeviceContext * devCon, int indexCount) {
		devCon->IASetInputLayout(mLayout);

		devCon->VSSetShader(mVertexShader, NULL, 0);
		devCon->PSSetShader(mPixelShader, NULL, 0);
		
		devCon->DrawIndexed(indexCount, 0,0);
	}
}