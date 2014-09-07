#include <time.h>

#include "ModelListClass.h"	

namespace Cobol {
	ModelListClass::ModelListClass() {
		mModelCount = 0;
	}
	ModelListClass::~ModelListClass() {
		Shutdown();
	}
	ModelListClass::ModelListClass(const ModelListClass&)
	{

	}

	bool ModelListClass::Init(int num)
	{
		float red, blue, green;
		mModelCount = num;
		mModelInfoList = new ModelInfoType[num];
		if (!mModelInfoList)
			return false;
		srand((unsigned int)time(NULL));
		for (int i = 0; i < num; ++i)
		{
			red = (float)rand() / RAND_MAX;
			blue = (float)rand() / RAND_MAX;
			green = (float)rand() / RAND_MAX;
		
			mModelInfoList[i].color = XMFLOAT4(red, blue, green, 1.0f);
			mModelInfoList[i].pos = XMFLOAT3((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f, (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f, (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f);
		}
		return true;
	}
	void ModelListClass::Shutdown() {
		delete[] mModelInfoList;
		mModelInfoList = NULL;
		mModelCount = 0;
	}
	int ModelListClass::GetModelCount() {
		return mModelCount;
	}
	void ModelListClass::GetDate(int ID, XMFLOAT3& pos, XMFLOAT4& color) {
		if (ID >= mModelCount)
			return;
		pos = mModelInfoList[ID].pos;
		color = mModelInfoList[ID].color;
	}
}