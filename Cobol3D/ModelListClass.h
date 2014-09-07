#ifndef __MODELLISTCLASS_H__
#define __MODELLISTCLASS_H__

#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )

namespace Cobol {
	class ModelListClass {
	private:
		struct ModelInfoType {
			XMFLOAT3 pos;
			XMFLOAT4 color;
		};
	public:
		ModelListClass();
		~ModelListClass();
		ModelListClass(const ModelListClass&);

		bool Init(int num);
		void Shutdown();

		int GetModelCount();
		void GetDate(int ID, XMFLOAT3& pos, XMFLOAT4& color);

	private:
		int mModelCount;
		ModelInfoType * mModelInfoList;
	};
}
#endif