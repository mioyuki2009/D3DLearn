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
	void SetTex(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }
	void SetLightDirection(const XMFLOAT3& v) { LightDirection->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetLightAmbient(const FXMVECTOR& M) { AmbientColor->SetFloatVector(reinterpret_cast<const float*>(&M)); }
	void SetLightColor(const FXMVECTOR& M) { DiffuseColor->SetFloatVector(reinterpret_cast<const float*>(&M)); }
	void SetSpecularColor(const FXMVECTOR& M) { SpecularColor->SetFloatVector(reinterpret_cast<const float*>(&M)); }
	void SetCameraPosition(const XMFLOAT3& v) { CameraPosition->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetSpecularPower(const float& f) { SpecularPower->SetFloat(f); }

	// water
	void SetReflectionMatrix(CXMMATRIX M) { ReflectionMatrix->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetReflectionTexture(ID3D11ShaderResourceView* tex) { ReflectionTexture->SetResource(tex); }
	void SetRefractionTexture(ID3D11ShaderResourceView* tex) { RefractionTexture->SetResource(tex); }
	void SetNormalTexture(ID3D11ShaderResourceView* tex) { NormalTexture->SetResource(tex); }
	void SetWaterTranslation(const float& f) { WaterTranslation->SetFloat(f); }
	void SetReflectRefractScale(const float& f) { ReflectRefractScale->SetFloat(f); }

	//Refract
	void SetClipPlane(const FXMVECTOR& M) { ClipPlane->SetFloatVector(reinterpret_cast<const float*>(&M)); }

	ID3DX11EffectTechnique* PassTech1;
	ID3DX11EffectTechnique* PassTech2;
	ID3DX11EffectTechnique* PassTech3;

	ID3DX11EffectMatrixVariable* WorldMat;
	ID3DX11EffectMatrixVariable* ViewMat;
	ID3DX11EffectMatrixVariable* ProjMat;

	ID3DX11EffectVectorVariable* LightDirection;
	ID3DX11EffectVectorVariable* AmbientColor;
	ID3DX11EffectVectorVariable* DiffuseColor;
	ID3DX11EffectVectorVariable* SpecularColor;
	ID3DX11EffectScalarVariable* SpecularPower;
	ID3DX11EffectVectorVariable* CameraPosition;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;

	// Water
	ID3DX11EffectMatrixVariable* ReflectionMatrix;
	ID3DX11EffectShaderResourceVariable* ReflectionTexture;
	ID3DX11EffectShaderResourceVariable* RefractionTexture;
	ID3DX11EffectShaderResourceVariable* NormalTexture;
	ID3DX11EffectScalarVariable* WaterTranslation;
	ID3DX11EffectScalarVariable* ReflectRefractScale;

	//Refract
	ID3DX11EffectVectorVariable* ClipPlane;

}; 

class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
};