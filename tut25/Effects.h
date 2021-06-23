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
	void SetClipPlane(const FXMVECTOR& M) { ClipPlane->SetFloatVector(reinterpret_cast<const float*>(&M)); }
	void SetTex(ID3D11ShaderResourceView* tex) { ShaderTexture->SetResource(tex); }
	void SetTextureTranslation(const float& f) { TextureTranslation->SetFloat(f); }

	ID3DX11EffectTechnique* PassTech;
	
	ID3DX11EffectMatrixVariable* WorldMat;
	ID3DX11EffectMatrixVariable* ViewMat;
	ID3DX11EffectMatrixVariable* ProjMat;
	ID3DX11EffectShaderResourceVariable* ShaderTexture;
	ID3DX11EffectVectorVariable* ClipPlane;
	ID3DX11EffectScalarVariable* TextureTranslation;

};

class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
};