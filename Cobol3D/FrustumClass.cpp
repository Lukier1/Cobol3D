#include "FrustumClass.h"

namespace Cobol {
	float dotProduct(XMFLOAT4 &P, XMFLOAT3 &V)
	{
		XMVECTOR vector = XMLoadFloat3(&V);
		XMFLOAT4 res;
		XMStoreFloat4(&res, XMPlaneDotCoord(XMLoadFloat4(&P), vector));
		return res.w;
	}
	
	FrustumClass::FrustumClass() {

	}
	FrustumClass::FrustumClass(const FrustumClass&)
	{

	}
	FrustumClass::~FrustumClass()
	{

	}

	void FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX &projectionMatrix, XMMATRIX	&viewMatrix)
	{
		float zMin, r;
		XMMATRIX matrix;

	
		zMin = -projectionMatrix._43 / projectionMatrix._33;
		r = screenDepth / (screenDepth - zMin);
		projectionMatrix._33 = r;
		projectionMatrix._43 = -r * zMin;
		matrix = viewMatrix*projectionMatrix;

		m_planes[0].x = matrix._14 + matrix._13;
		m_planes[0].y = matrix._24 + matrix._23;
		m_planes[0].z = matrix._34 + matrix._33;
		m_planes[0].w = matrix._44 + matrix._43;
		XMStoreFloat4(&m_planes[0], XMPlaneNormalize(XMLoadFloat4(&m_planes[0])));
		

		// Calculate far plane of frustum.
		m_planes[1].x = matrix._14 - matrix._13;
		m_planes[1].y = matrix._24 - matrix._23;
		m_planes[1].z = matrix._34 - matrix._33;
		m_planes[1].w = matrix._44 - matrix._43;
		XMStoreFloat4(&m_planes[1], XMPlaneNormalize(XMLoadFloat4(&m_planes[1])));

		// Calculate left plane of frustum.
		m_planes[2].x = matrix._14 + matrix._11;
		m_planes[2].y = matrix._24 + matrix._21;
		m_planes[2].z = matrix._34 + matrix._31;
		m_planes[2].w = matrix._44 + matrix._41;
		XMStoreFloat4(&m_planes[2], XMPlaneNormalize(XMLoadFloat4(&m_planes[2])));

		// Calculate right plane of frustum.
		m_planes[3].x = matrix._14 - matrix._11;
		m_planes[3].y = matrix._24 - matrix._21;
		m_planes[3].z = matrix._34 - matrix._31;
		m_planes[3].w = matrix._44 - matrix._41;
		XMStoreFloat4(&m_planes[3], XMPlaneNormalize(XMLoadFloat4(&m_planes[3])));

		// Calculate top plane of frustum.
		m_planes[4].x = matrix._14 - matrix._12;
		m_planes[4].y = matrix._24 - matrix._22;
		m_planes[4].z = matrix._34 - matrix._32;
		m_planes[4].w = matrix._44 - matrix._42;
		XMStoreFloat4(&m_planes[4], XMPlaneNormalize(XMLoadFloat4(&m_planes[4])));

		// Calculate bottom plane of frustum.
		m_planes[5].x = matrix._14 + matrix._12;
		m_planes[5].y = matrix._24 + matrix._22;
		m_planes[5].z = matrix._34 + matrix._32;
		m_planes[5].w = matrix._44 + matrix._42;
		XMStoreFloat4(&m_planes[5], XMPlaneNormalize(XMLoadFloat4(&m_planes[5])));
	}

	bool FrustumClass::CheckPoint(float x, float y , float z)
	{
		XMVECTOR vector=XMLoadFloat3(&XMFLOAT3(x, y, z));
		XMFLOAT4 res;
		for (int i = 0; i < 6; ++i)
		{
			XMStoreFloat4(&res, XMPlaneDotCoord(XMLoadFloat4(&m_planes[i]), vector));
			if(res.w < 0.0f)
				return false;	
		}
		return true;
	}
	bool FrustumClass::CheckCube(float x, float y, float z, float radius)
	{
		int i;
		// Check if any one point of the cube is in the view frustum.
		for (i = 0; i<6; i++)
		{
			if (dotProduct(m_planes[i], XMFLOAT3((x - radius), (y - radius), (z - radius))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((x + radius), (y - radius), (z - radius))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((x - radius), (y + radius), (z - radius))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((x + radius), (y + radius), (z - radius))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((x - radius), (y - radius), (z + radius))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((x + radius), (y - radius), (z + radius))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((x - radius), (y + radius), (z + radius))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((x + radius), (y + radius), (z + radius))) >= 0.0f)
			{
				continue;
			}

			return false;
		}

		return true;
	}
	bool FrustumClass::CheckSphere(float x, float y, float z, float r)
	{
		int i;


		// Check if the radius of the sphere is inside the view frustum.
		for (i = 0; i<6; i++)
		{
			if (dotProduct(m_planes[i], XMFLOAT3(x, y, z)) < -r)
			{
				return false;
			}
		}

		return true;
	}
	bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
	{
		int i;


		// Check if any of the 6 planes of the rectangle are inside the view frustum.
		for (i = 0; i<6; i++)
		{
			if (dotProduct(m_planes[i], XMFLOAT3((xCenter - xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((xCenter + xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((xCenter - xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((xCenter - xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((xCenter + xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((xCenter + xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((xCenter - xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
			{
				continue;
			}

			if (dotProduct(m_planes[i], XMFLOAT3((xCenter + xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
			{
				continue;
			}

			return false;
		}
		return true;
	}


}