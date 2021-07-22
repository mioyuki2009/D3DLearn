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
	
	void SetViewMatrix2(CXMMATRIX M) { ViewMatrix2->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetProjectionMatrix2(CXMMATRIX M) { ProjectionMatrix2->SetMatrix(reinterpret_cast<const float*>(&M)); }

	void SetTexture(ID3D11ShaderResourceView* tex) { Texture->SetResource(tex); }
	void SetProjectionTexture(ID3D11ShaderResourceView* tex) { ProjectionTexture->SetResource(tex); }

	void SetLightPosition(const XMFLOAT3& v) { LightPosition->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetAmbientColor(const FXMVECTOR& M) { AmbientColor->SetFloatVector(reinterpret_cast<const float*>(&M)); }
	void SetDiffuseColor(const FXMVECTOR& M) { DiffuseColor->SetFloatVector(reinterpret_cast<const float*>(&M)); }

	ID3DX11EffectTechnique* PassTech1;

	ID3DX11EffectMatrixVariable* WorldMat;
	ID3DX11EffectMatrixVariable* ViewMat;
	ID3DX11EffectMatrixVariable* ProjMat;

	ID3DX11EffectMatrixVariable* ViewMatrix2;
	ID3DX11EffectMatrixVariable* ProjectionMatrix2;

	ID3DX11EffectShaderResourceVariable* Texture;
	ID3DX11EffectShaderResourceVariable* ProjectionTexture;

	ID3DX11EffectVectorVariable* LightPosition;
	ID3DX11EffectVectorVariable* AmbientColor;
	ID3DX11EffectVectorVariable* DiffuseColor;
};

class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
};