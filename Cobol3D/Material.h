#ifndef __MATERIAL_H__
#define __MATERIAL_H__


#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )

#include "MyMath.h"



class Material {
public:
	Material(ID3D11Device * device,ID3D11DeviceContext * _devCon) : devCon(_devCon)
	{
		md3dDevice = device;
		InitPipeLine();
	}

	void InitPipeLine()
	{
		ID3DBlob *VS, *PS;
		HRESULT result;
		D3DX11CompileFromFile("shaders.hlsl", 0 ,0, "VS_Main", "vs_5_0", 0, 0, 0, &VS, 0,&result);
		if(FAILED(result))
		{
			DXTRACE_MSG( "Failed to compile VS_Main" );
		}
		else
			DXTRACE_MSG( "Succes to compile VS_Main" );
		D3DX11CompileFromFile("shaders.hlsl", 0 ,0, "PS_Main", "ps_5_0", 0, 0, 0, &PS, 0,&result);
		if(FAILED(result))
		{
			DXTRACE_MSG( "Failed to compile PS_Main" );
		}
		else
			DXTRACE_MSG( "Succes to compile PS_Main" );
		md3dDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &mVS);
		md3dDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &mPS);

		
		
		D3D11_INPUT_ELEMENT_DESC ied[] = 
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		md3dDevice->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &mInpLay);
	}
	
	void UseMaterial()
	{
		devCon->IASetInputLayout(mInpLay);
		devCon->VSSetShader(mVS, 0, 0);
		devCon->PSSetShader(mPS, 0, 0);
	}

	void LoadContent(){
		
	}
	
	void UnloadContent() {

		if( mVS ) mVS->Release( );
		if( mPS )  mPS->Release( );
		if( mInpLay ) mInpLay->Release( );
		if( mVertexBuffer ) mVertexBuffer->Release( );
		if( mIndexBuffer ) mIndexBuffer->Release( );
		
	
		mInpLay = 0;
	
	}

	
protected:
	ID3D11DeviceContext * devCon;
	ID3D11VertexShader * mVS;
	ID3D11PixelShader * mPS; 
	ID3D11InputLayout * mInpLay;
	ID3D11Buffer * mVertexBuffer, *mIndexBuffer;

	
	ID3D11Device * md3dDevice;
};

#endif