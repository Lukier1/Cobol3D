#ifndef __FRUSTUMCLASS_H__
#define __FRUSTUMCLASS_H__
#pragma warning ( disable : 4005 )
#include<dxgi.h>

#include<d3dx11.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )
namespace Cobol {
	class FrustumClass {
	public:
		FrustumClass();
		FrustumClass(const FrustumClass&);
		~FrustumClass();

		void ConstructFrustum(float, XMMATRIX&, XMMATRIX&);

		bool CheckPoint(float, float, float);
		bool CheckCube(float, float, float, float);
		bool CheckSphere(float, float, float, float);
		bool CheckRectangle(float, float, float, float, float, float);
	private:
		XMFLOAT4 m_planes[6];

	};
}
#endif