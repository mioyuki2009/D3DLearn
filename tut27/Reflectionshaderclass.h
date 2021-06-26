#pragma once
#include <d3d11.h>
#include <Macro.hpp>
#include <xnamath.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;

class Reflectionshaderclass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	Reflectionshaderclass();
	~Reflectionshaderclass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*,
		ID3D11ShaderResourceView*, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND);
	void ShutdownShader();
	
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*,
		ID3D11ShaderResourceView*, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11InputLayout* m_layout;
	
};