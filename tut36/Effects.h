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
	
	void SetShaderTexture(ID3D11ShaderResourceView* tex) { ShaderTexture->SetResource(tex); }
	
	void SetScreenWidth(const float& f) { ScreenWidth->SetFloat(f); }
	void SetScreenHeight(const float& f) { ScreenHeight->SetFloat(f); }
	
	ID3DX11EffectTechnique* PassTech1;
	ID3DX11EffectTechnique* PassTech2;
	ID3DX11EffectTechnique* PassTechTexture;

	ID3DX11EffectMatrixVariable* WorldMat;
	ID3DX11EffectMatrixVariable* ViewMat;
	ID3DX11EffectMatrixVariable* ProjMat;

	ID3DX11EffectShaderResourceVariable* ShaderTexture;

	ID3DX11EffectScalarVariable* ScreenWidth;
	ID3DX11EffectScalarVariable* ScreenHeight;
};

class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
};