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
	
	void SetFireTexture(ID3D11ShaderResourceView* tex) { FireTexture->SetResource(tex); }
	void SetNoiseTexture(ID3D11ShaderResourceView* tex) { NoiseTexture->SetResource(tex); }
	void SetAlphaTexture(ID3D11ShaderResourceView* tex) { AlphaTexture->SetResource(tex); }
	
	void SetFrameTime(const float& f) { FrameTime->SetFloat(f); }
	void SetScrollSpeeds(const XMFLOAT3& v) { ScrollSpeeds->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetScales(const XMFLOAT3& v) { Scales->SetRawValue(&v, 0, sizeof(XMFLOAT3));}

	void SetDistortion1(const XMFLOAT2& v) { Distortion1->SetRawValue(&v, 0, sizeof(XMFLOAT2)); }
	void SetDistortion2(const XMFLOAT2& v) { Distortion2->SetRawValue(&v, 0, sizeof(XMFLOAT2)); }
	void SetDistortion3(const XMFLOAT2& v) { Distortion3->SetRawValue(&v, 0, sizeof(XMFLOAT2)); }

	void SetDistortionScale(const float& f) { DistortionScale->SetFloat(f); }
	void SetDistortionBias(const float& f) { DistortionBias->SetFloat(f); }

	ID3DX11EffectTechnique* PassTech;

	ID3DX11EffectMatrixVariable* WorldMat;
	ID3DX11EffectMatrixVariable* ViewMat;
	ID3DX11EffectMatrixVariable* ProjMat;

	ID3DX11EffectShaderResourceVariable* FireTexture;
	ID3DX11EffectShaderResourceVariable* NoiseTexture;
	ID3DX11EffectShaderResourceVariable* AlphaTexture;

	ID3DX11EffectScalarVariable* FrameTime;
	ID3DX11EffectVectorVariable* ScrollSpeeds;
	ID3DX11EffectVectorVariable* Scales;

	ID3DX11EffectVectorVariable* Distortion1;
	ID3DX11EffectVectorVariable* Distortion2;
	ID3DX11EffectVectorVariable* Distortion3;

	ID3DX11EffectScalarVariable* DistortionScale;
	ID3DX11EffectScalarVariable* DistortionBias;
};

class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
};