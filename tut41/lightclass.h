#pragma once

#include <d3d11.h>
#include <XNAMath.h>
class LightClass
{
public:
	LightClass();
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);

	void SetPosition(float, float, float);
	void SetLookAt(float, float, float);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT4 GetAmbientColor();
	XMFLOAT3 GetPosition();

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float, float);

	void GetViewMatrix(XMMATRIX&);
	void GetProjectionMatrix(XMMATRIX&);

private:
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_position;
	XMFLOAT3 m_lookAt;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;

};
