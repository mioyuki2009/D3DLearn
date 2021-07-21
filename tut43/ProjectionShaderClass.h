#pragma once
#include <d3d11.h>
#include <Macro.hpp>
#include <xnamath.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;

class ProjectionShaderClass
{

public:
	ProjectionShaderClass();
	~ProjectionShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, 
		XMFLOAT4, XMFLOAT4, XMFLOAT3, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device*, HWND);
	void ShutdownShader();
	
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*,
		XMFLOAT4, XMFLOAT4, XMFLOAT3, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11InputLayout* m_layout;
	
};