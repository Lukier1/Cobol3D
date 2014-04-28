#include "Camera.h"

namespace Cobol {
	Camera::Camera() {
		mPosition = XMFLOAT3(0,0,0);
		mUp = XMFLOAT3(0,1.0f,0);
		mLookAt = XMFLOAT3(0,0,1.0f);
		mRotation = XMFLOAT3(0,0,0);
		mViewMatrix = XMMatrixIdentity();
	}

	void Camera::Render() {
		//mPosition.z+=0.1f;
		XMFLOAT3 newRot=mRotation,lookAt = mLookAt;
		XMMATRIX rotationMatrix;
		newRot.x*=(float)((2*3.14/180));
		newRot.y*=(float)((2*3.14/180));
		newRot.z*=(float)((2*3.14/180));
		rotationMatrix = XMMatrixRotationRollPitchYaw(newRot.x,newRot.y, newRot.z);
		//XMVector3TransformCoord( XMLoadFloat3(&mLookAt), rotationMatrix);
		//XMVector3TransformCoord( XMLoadFloat3(&mUp), rotationMatrix);

		lookAt.x+=mPosition.x; 
		lookAt.y+=mPosition.y; 
		lookAt.z+=mPosition.z; 

		rotationMatrix *= XMMatrixLookAtLH(XMLoadFloat3(&mPosition),XMLoadFloat3(&lookAt), XMLoadFloat3(&mUp));
		mViewMatrix = rotationMatrix;
	}
};