#pragma warning ( disable : 4005 )
#include<dxgi.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#pragma warning ( default : 4005 )
#ifndef __CAMERA_H__
#define __CAMERA_H__


namespace Cobol {
	class Camera {
	private:
		XMFLOAT3 mPosition, mUp, mLookAt, mRotation; // rotation[3] = {  yaw, pith ,roll }
		XMMATRIX mViewMatrix;
	public:
		void getMatrix(XMMATRIX& mat) { mat = mViewMatrix; }

		void Render();
		XMFLOAT3 GetPosition() { return mPosition; }
		XMFLOAT3 GetRotation() { return mRotation; }
		
		void SetRotation(XMFLOAT3 r) { mRotation = r; }
		void SetPosition(XMFLOAT3 p) { mPosition = p; }

		Camera();
		~Camera() { }
	};

};

#endif