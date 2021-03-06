#ifndef __MODELCLASS_H__
#define __MODELCLASS_H__

#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )

#include "TextureClass.h"

#include <fstream>

namespace Cobol {
	class ModelClass {
	private:
		struct VertexType {
			XMFLOAT3 position;
			XMFLOAT2 texture;
			XMFLOAT3 normal;
			XMFLOAT3 tangent;
			XMFLOAT3 binormal;
		};

		struct ModelType {
			float x,y,z;
			float tu, tv;
			float nx, ny, nz;
			float tx, ty, tz;
			float bx, by, bz;
		};
		struct TempVertexType {
			float x, y, z;
			float tu, tv;
			float nx, ny, nz;
		};
		struct VectorType {
			float x, y, z;
		};

	public:
		ModelClass();
		ModelClass(const ModelClass&);
		~ModelClass(); 

		bool Init(ID3D11Device*,char*, WCHAR*);
		void Shutdown();
		void Render(ID3D11DeviceContext*);

		int GetIndexCount();
		ID3D11ShaderResourceView * getTexture();

	private:
		void ShutdownBuffers();
		bool InitBuffers(ID3D11Device*);
		void RenderBuffer(ID3D11DeviceContext*);
		bool LoadTexture(ID3D11Device*, char*);
		void ReleaseTexture();

		bool LoadModel(WCHAR*);
		void ReleaseModel();

		void CalculateModelVectors();
		void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, XMFLOAT3&, XMFLOAT3&);
		void CalculateNormal(XMFLOAT3, XMFLOAT3, XMFLOAT3&);
		ID3D11Buffer * mVertexBuffer, *mIndexBuffer;
		int mVertexCount, mIndexCount;

		TextureClass * mTexture;

		ModelType * mModel;
	};
}
#endif