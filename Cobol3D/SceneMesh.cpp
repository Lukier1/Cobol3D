#include "SceneMesh.h"

void SceneMesh::DrawMesh(ID3D11Device * device, ID3D11DeviceContext * context) {
	mMaterial->UseMaterial();
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	ID3D11Buffer * buff = mMesh->getVertexBuffer();
	context->IASetVertexBuffers(0, 1, &buff, &stride, &offset);
	context->IASetIndexBuffer(mMesh->getIndexBuffer(), DXGI_FORMAT_R16_UINT, 0 );
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->DrawIndexed(36,0,0);
}

XMMATRIX SceneMesh::getTransform() {
	if(mRot[2] < 6.28f) {
		//mRot[2]+=0.001f; 
		mRot[2]=1.5f;
		//mRot[0]+=0.001f;
//	mRot[1]+=0.001f;//
		mRot[1]=1.0f;
	}
	else{
		mRot[2]=0.0f;
		mRot[0]=0.0f;
	}
	XMMATRIX rotationMatY = XMMatrixRotationY(mRot[1]);///XMMatrixRotationRollPitchYaw(mRot[0],mRot[1],mRot[2]);	// Obrót mesha 
	XMMATRIX rotationMatX = XMMatrixRotationX(mRot[0]);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(mRot[0],mRot[1],mRot[2]);
	XMMATRIX translationMat = XMMatrixTranslation(mPos[0],mPos[1],mPos[2]);	// Pozycja mesha
		
	XMMATRIX worldMat;
	worldMat =  rotation*translationMat ;
	return worldMat;
}