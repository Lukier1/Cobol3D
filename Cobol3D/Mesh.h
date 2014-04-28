#ifndef __MESH_H__
#define __MESH_H__

#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )

class VERTEX {
public:
	XMFLOAT3 pos;
	XMFLOAT3 norm;
	float color[4];
};
class Mesh
{
protected:
	
	VERTEX * vertices;
	ID3D11Device * mDevice;
	ID3D11DeviceContext * mDevCon;
	ID3D11Buffer * mVertexBuffer, * mIndexBuffer;
	
public:
	Mesh() : vertices(NULL), mVertexBuffer(NULL) {
	//	LoadContent();
	}
	virtual void LoadContent(ID3D11Device * device, ID3D11DeviceContext * devCon) = NULL;
	ID3D11Buffer * getVertexBuffer() const {
		return mVertexBuffer;
	}
	ID3D11Buffer * getIndexBuffer() const {
		return mIndexBuffer;
	}
};

class CubeMesh : public Mesh {

public:
	void LoadContent(ID3D11Device * device, ID3D11DeviceContext * devCon) {
		mDevice=device;
		mDevCon=devCon;

		VERTEX vertices[] =
    {
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },
        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},

        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ),{1.0f, 0.0f, 0.0f, 1.0f} },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) ,{1.0f, 0.0f, 0.0f, 1.0f}},
    };
		D3D11_BUFFER_DESC  bd;
		ZeroMemory(&bd, sizeof(bd));
		
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX) * 24;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData,sizeof(initData));
		initData.pSysMem = vertices;
		mDevice->CreateBuffer(&bd, &initData, &mVertexBuffer);

		WORD indices[] =
		 {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
		 };
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD)*36;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		initData.pSysMem = indices;
		mDevice->CreateBuffer(&bd, &initData, &mIndexBuffer);
	}

};

#endif