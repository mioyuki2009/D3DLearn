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
	void SetFontColor(const FXMVECTOR& M) { FontColor->SetFloatVector(reinterpret_cast<const float*>(&M)); }

	ID3DX11EffectTechnique* PassTech;

	ID3DX11EffectVectorVariable* FontColor;
	ID3DX11EffectMatrixVariable* WorldMat;
	ID3DX11EffectMatrixVariable* ViewMat;
	ID3DX11EffectMatrixVariable* ProjMat;
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
};

class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
};