#include "Frustumclass.h"

FrustumClass::FrustumClass()
{
}

FrustumClass::~FrustumClass()
{
}

void FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	float zMinimum, r;
	XMMATRIX matrix;
	
	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	matrix = XMMatrixMultiply(viewMatrix, projectionMatrix);
	
	// Calculate near plane of frustum.
	m_planes[0].x = matrix._14 + matrix._13;
	m_planes[0].y = matrix._24 + matrix._23;
	m_planes[0].z = matrix._34 + matrix._33;
	m_planes[0].w = matrix._44 + matrix._43;

	XMVECTOR m_planesVec = XMLoadFloat4(&(m_planes[0]));
	m_planesVec = XMPlaneNormalize(m_planesVec);
	XMStoreFloat4(&(m_planes[0]), m_planesVec);

	// Calculate far plane of frustum.
	m_planes[1].x = matrix._14 - matrix._13;
	m_planes[1].y = matrix._24 - matrix._23;
	m_planes[1].z = matrix._34 - matrix._33;
	m_planes[1].w = matrix._44 - matrix._43;
	m_planesVec = XMLoadFloat4(&(m_planes[1]));
	m_planesVec = XMPlaneNormalize(m_planesVec);
	XMStoreFloat4(&(m_planes[1]), m_planesVec);

	// Calculate left plane of frustum.
	m_planes[2].x = matrix._14 + matrix._11;
	m_planes[2].y = matrix._24 + matrix._21;
	m_planes[2].z = matrix._34 + matrix._31;
	m_planes[2].w = matrix._44 + matrix._41;
	m_planesVec = XMLoadFloat4(&(m_planes[2]));
	m_planesVec = XMPlaneNormalize(m_planesVec);
	XMStoreFloat4(&(m_planes[2]), m_planesVec);

	// Calculate right plane of frustum.
	m_planes[3].x = matrix._14 - matrix._11;
	m_planes[3].y = matrix._24 - matrix._21;
	m_planes[3].z = matrix._34 - matrix._31;
	m_planes[3].w = matrix._44 - matrix._41;
	m_planesVec = XMLoadFloat4(&(m_planes[3]));
	m_planesVec = XMPlaneNormalize(m_planesVec);
	XMStoreFloat4(&(m_planes[3]), m_planesVec);

	// Calculate top plane of frustum.
	m_planes[4].x = matrix._14 - matrix._12;
	m_planes[4].y = matrix._24 - matrix._22;
	m_planes[4].z = matrix._34 - matrix._32;
	m_planes[4].w = matrix._44 - matrix._42;
	m_planesVec = XMLoadFloat4(&(m_planes[4]));
	m_planesVec = XMPlaneNormalize(m_planesVec);
	XMStoreFloat4(&(m_planes[4]), m_planesVec);

	// Calculate bottom plane of frustum.
	m_planes[5].x = matrix._14 + matrix._12;
	m_planes[5].y = matrix._24 + matrix._22;
	m_planes[5].z = matrix._34 + matrix._32;
	m_planes[5].w = matrix._44 + matrix._42;
	m_planesVec = XMLoadFloat4(&(m_planes[5]));
	m_planesVec = XMPlaneNormalize(m_planesVec);
	XMStoreFloat4(&(m_planes[5]), m_planesVec);
	return;
}

bool FrustumClass::CheckPoint(float x, float y, float z)
{
	int i;


	// Check if the point is inside all six planes of the view frustum.
	for (i = 0; i < 6; i++)
	{
		if (PlaneDotCoord(m_planes[i], XMFLOAT4(x, y, z, 1)) < 0.0f)
		{
			return false;
		}
	}

	return true;
}

float FrustumClass::PlaneDotCoord(XMFLOAT4 plane, XMFLOAT4 coor) {
	XMVECTOR m_planesVec = XMLoadFloat4(&plane);
	XMVECTOR m_Vec3 = XMLoadFloat4(&coor);
	XMVECTOR vectorOut = XMPlaneDotCoord(m_planesVec, m_Vec3);
	XMFLOAT4 ans = XMFLOAT4(0.0f,0.0f,0.0f,0.0f);
	XMStoreFloat4(&ans, vectorOut);
	return ans.x;
}


bool FrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;


	// Check if any one point of the cube is in the view frustum.
	for (i = 0; i < 6; i++)
	{
		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter - radius), (yCenter - radius), (zCenter - radius), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter + radius), (yCenter - radius), (zCenter - radius), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter - radius), (yCenter + radius), (zCenter - radius), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter + radius), (yCenter + radius), (zCenter - radius), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter - radius), (yCenter - radius), (zCenter + radius), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter + radius), (yCenter - radius), (zCenter + radius), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter - radius), (yCenter + radius), (zCenter + radius), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter + radius), (yCenter + radius), (zCenter + radius), 1.0f)) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool FrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;


	// Check if the radius of the sphere is inside the view frustum.
	for (i = 0; i < 6; i++)
	{
		if (PlaneDotCoord(m_planes[i], XMFLOAT4(xCenter, yCenter, zCenter,1.0f)) < -radius)
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
	for (i = 0; i < 6; i++)
	{
		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter - xSize), (yCenter - ySize), (zCenter - zSize),1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1.0f)) >= 0.0f)
		{
			continue;
		}

		if (PlaneDotCoord(m_planes[i], XMFLOAT4((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1.0f)) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}
