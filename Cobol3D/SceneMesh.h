#ifndef __SCENEMESH_H__
#define __SCENEMESH_H__

#include "Mesh.h"
#include "Material.h"

class SceneMesh {
protected:
	Material * mMaterial;
	Mesh * mMesh;
	float mRot[3];
	 float mPos[3];
public:
	SceneMesh() : mMaterial(0), mMesh(0) {
		for(int i = 0; i < 3; i++)
			mRot[i] = mPos[i] = 0;
	}
	void setMaterial(Material * material) {
		mMaterial = material;
		
	}
	void setPosition( float pos[3])	{ mPos[0]=pos[0]; mPos[1]=pos[1]; mPos[2]=pos[2]; }
//	float[3] getPosition() { return mPosition; }
	void setRotation(float rot[3]) { mRot[0]=rot[0]; mRot[1]=rot[1]; mRot[2]=rot[2]; }
	void setMesh(Mesh * mesh)
	{
		mMesh = mesh;
	}
	XMMATRIX getTransform();

	void DrawMesh(ID3D11Device * device, ID3D11DeviceContext * context);
	
};
#endif