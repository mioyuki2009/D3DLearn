#pragma once

#include <d3d11.h>
#include <XNAMath.h>
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetPosition(float, float, float);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT4 GetPosition();

private:
	XMFLOAT4 m_diffuseColor;
	XMFLOAT4 m_position;
};
