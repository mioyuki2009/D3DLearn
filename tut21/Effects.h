#pragma once
#include <d3d11.h>
#include <xnamath.h>
#include <xstring>
#include "d3dx11effect.h"
class Effect
{
public:
	Effect(ID3D11Device* device, const std::string& filename);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);

protected:
	ID3DX11Effect* mFX;
};

class BasicEffect : public Effect
{
public:
	BasicEffect(ID3D11Device* device, const std::string& filename);
	~BasicEffect();

	void SetWorldMatrix(CXMMATRIX M) { WorldMat->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetViewMatrix(CXMMATRIX M) { ViewMat->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetProjMatrix(CXMMATRIX M) { ProjMat->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTex1(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }
	void SetTex2(ID3D11ShaderResourceView* tex) { BumpMap->SetResource(tex); }
	void SetTex3(ID3D11ShaderResourceView* tex) { SpecMap->SetResource(tex); }
	void SetLightDirection(const XMFLOAT3& v) { LightDirection->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetLightColor(const FXMVECTOR& M) { DiffuseColor->SetFloatVector(reinterpret_cast<const float*>(&M)); }

	void SetCameraPosition(const XMFLOAT3& v) { CameraPosition->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetSpecularColor(const FXMVECTOR& M) { SpecularColor->SetFloatVector(reinterpret_cast<const float*>(&M)); }
	void SetSpecularPower(const float& f) { SpecularPower->SetFloat(f); }

	ID3DX11EffectTechnique* PassTech;

	ID3DX11EffectMatrixVariable* WorldMat;
	ID3DX11EffectMatrixVariable* ViewMat;
	ID3DX11EffectMatrixVariable* ProjMat;

	ID3DX11EffectVectorVariable* LightDirection;
	ID3DX11EffectVectorVariable* DiffuseColor;

	ID3DX11EffectVectorVariable* CameraPosition;
	ID3DX11EffectVectorVariable* SpecularColor;
	ID3DX11EffectScalarVariable* SpecularPower;


	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* BumpMap;
	ID3DX11EffectShaderResourceVariable* SpecMap;
};

class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
};