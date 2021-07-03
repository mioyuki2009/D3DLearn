#include "Effects.h"
#include "fstream"
#include "vector"
#include "Macro.hpp"

#pragma region Effect
Effect::Effect(ID3D11Device* device, const std::string& filename)
	: mFX(0)
{
	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, device, &mFX));
}

Effect::~Effect()
{
	ReleaseCOM(mFX);
}
#pragma endregion

#pragma region BasicEffect
BasicEffect::BasicEffect(ID3D11Device* device, const std::string& filename)
	: Effect(device, filename)
{
	PassTech = mFX->GetTechniqueByName("Pass1");
	PassTechTexture = mFX->GetTechniqueByName("Pass2");

	WorldMat = mFX->GetVariableByName("worldMatrix")->AsMatrix();
	ViewMat = mFX->GetVariableByName("viewMatrix")->AsMatrix();
	ProjMat = mFX->GetVariableByName("projectionMatrix")->AsMatrix();
	
	ColorTexture = mFX->GetVariableByName("colorTexture")->AsShaderResource();
	NormalTexture = mFX->GetVariableByName("normalTexture")->AsShaderResource();
	RefractionTexture = mFX->GetVariableByName("refractionTexture")->AsShaderResource();
	RefractionScale = mFX->GetVariableByName("refractionScale")->AsScalar();

	ShaderTexture = mFX->GetVariableByName("shaderTexture")->AsShaderResource();
}

BasicEffect::~BasicEffect()
{
}
#pragma endregion

#pragma region Effects

BasicEffect* Effects::BasicFX = 0;

void Effects::InitAll(ID3D11Device* device)
{
	BasicFX = new BasicEffect(device, "./Basic.fxo");
}

void Effects::DestroyAll()
{
	SafeDelete(BasicFX);
}
#pragma endregion