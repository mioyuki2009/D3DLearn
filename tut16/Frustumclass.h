#pragma once

#include <d3d11.h>
#include <XNAMath.h>

class FrustumClass
{
public:
	FrustumClass();
	~FrustumClass();

	void ConstructFrustum(float, XMMATRIX, XMMATRIX);

	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);

protected:
	float PlaneDotCoord(XMFLOAT4, XMFLOAT4);

private:
	XMFLOAT4 m_planes[6];
};

