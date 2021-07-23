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
	PassTech1 = mFX->GetTechniqueByName("Pass1");
	PassTech2 = mFX->GetTechniqueByName("Pass2");
	PassTech3 = mFX->GetTechniqueByName("Pass3");
	PassTech4 = mFX->GetTechniqueByName("Pass4");
	PassTech5 = mFX->GetTechniqueByName("Pass5");

	WorldMat = mFX->GetVariableByName("worldMatrix")->AsMatrix();
	ViewMat = mFX->GetVariableByName("viewMatrix")->AsMatrix();
	ProjMat = mFX->GetVariableByName("projectionMatrix")->AsMatrix();

	Texture = mFX->GetVariableByName("shaderTexture")->AsShaderResource();
	GlowMap = mFX->GetVariableByName("glowMapTexture")->AsShaderResource();

	ColorTexture = mFX->GetVariableByName("colorTexture")->AsShaderResource();
	GlowTexture = mFX->GetVariableByName("glowTexture")->AsShaderResource();

	GlowStrength = mFX->GetVariableByName("glowStrength")->AsScalar();

	ScreenWidth = mFX->GetVariableByName("screenWidth")->AsScalar();
	ScreenHeight = mFX->GetVariableByName("screenHeight")->AsScalar();

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