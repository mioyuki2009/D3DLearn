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
	
	void SetLightViewMatrix(CXMMATRIX M) { LightViewMatrix->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetLightProjectionMatrix(CXMMATRIX M) { LightProjectionMatrix->SetMatrix(reinterpret_cast<const float*>(&M)); }

	void SetTexture(ID3D11ShaderResourceView* tex) { Texture->SetResource(tex); }
	void SetDepthMapTexture(ID3D11ShaderResourceView* tex) { DepthMapTexture->SetResource(tex); }

	void SetLightPosition(const XMFLOAT3& v) { LightPosition->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetAmbientColor(const FXMVECTOR& M) { AmbientColor->SetFloatVector(reinterpret_cast<const float*>(&M)); }
	void SetDiffuseColor(const FXMVECTOR& M) { DiffuseColor->SetFloatVector(reinterpret_cast<const float*>(&M)); }
	
	void SetLightViewMatrix2(CXMMATRIX M) { LightViewMatrix2->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetLightProjectionMatrix2(CXMMATRIX M) { LightProjectionMatrix2->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetDepthMapTexture2(ID3D11ShaderResourceView* tex) { DepthMapTexture2->SetResource(tex); }
	void SetLightPosition2(const XMFLOAT3& v) { LightPosition2->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDiffuseColor2(const FXMVECTOR& M) { DiffuseColor2->SetFloatVector(reinterpret_cast<const float*>(&M)); }

	ID3DX11EffectTechnique* PassTech1;
	ID3DX11EffectTechnique* PassTech2;

	ID3DX11EffectMatrixVariable* WorldMat;
	ID3DX11EffectMatrixVariable* ViewMat;
	ID3DX11EffectMatrixVariable* ProjMat;

	ID3DX11EffectMatrixVariable* LightViewMatrix;
	ID3DX11EffectMatrixVariable* LightProjectionMatrix;

	ID3DX11EffectShaderResourceVariable* Texture;
	ID3DX11EffectShaderResourceVariable* DepthMapTexture;

	ID3DX11EffectVectorVariable* LightPosition;
	ID3DX11EffectVectorVariable* AmbientColor;
	ID3DX11EffectVectorVariable* DiffuseColor;

	ID3DX11EffectMatrixVariable* LightViewMatrix2;
	ID3DX11EffectMatrixVariable* LightProjectionMatrix2;
	ID3DX11EffectShaderResourceVariable* DepthMapTexture2;
	ID3DX11EffectVectorVariable* LightPosition2;
	ID3DX11EffectVectorVariable* DiffuseColor2;
};

class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
};