#include "FireShaderClass.h"
#include "Effects.h"
FireShaderClass::FireShaderClass()
{
	m_layout = 0;
}


FireShaderClass::~FireShaderClass()
{
}


bool FireShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	
	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd);
	if (!result)
	{
		return false;
	}
	return true;
}

void FireShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

bool FireShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* fireTexture,
	ID3D11ShaderResourceView* noiseTexture, ID3D11ShaderResourceView* alphaTexture,
	float frameTime, XMFLOAT3 scrollSpeeds, XMFLOAT3 scales, XMFLOAT2 distortion1, XMFLOAT2 distortion2,
	XMFLOAT2 distortion3, float distortionScale, float distortionBias)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, fireTexture,
		noiseTexture, alphaTexture, frameTime, scrollSpeeds, scales, distortion1, distortion2, distortion3,
		distortionScale, distortionBias);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

bool FireShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	Effects::InitAll(device);

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	D3DX11_PASS_DESC passDesc;
	Effects::BasicFX->PassTech->GetPassByIndex(0)->GetDesc(&passDesc);
	result = device->CreateInputLayout(polygonLayout, numElements, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_layout);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void FireShaderClass::ShutdownShader()
{

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	return;
}

bool FireShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* fireTexture,
	ID3D11ShaderResourceView* noiseTexture, ID3D11ShaderResourceView* alphaTexture,
	float frameTime, XMFLOAT3 scrollSpeeds, XMFLOAT3 scales, XMFLOAT2 distortion1, XMFLOAT2 distortion2,
	XMFLOAT2 distortion3, float distortionScale, float distortionBias)
{
	Effects::BasicFX->SetWorldMatrix(worldMatrix);
	Effects::BasicFX->SetViewMatrix(viewMatrix);
	Effects::BasicFX->SetProjMatrix(projectionMatrix);

	Effects::BasicFX->SetFireTexture(fireTexture);
	Effects::BasicFX->SetNoiseTexture(noiseTexture);
	Effects::BasicFX->SetAlphaTexture(alphaTexture);

	Effects::BasicFX->SetFrameTime(frameTime);
	Effects::BasicFX->SetScrollSpeeds(scrollSpeeds);
	Effects::BasicFX->SetScales(scales);

	Effects::BasicFX->SetDistortion1(distortion1);
	Effects::BasicFX->SetDistortion2(distortion2);
	Effects::BasicFX->SetDistortion3(distortion3);

	Effects::BasicFX->SetDistortionScale(distortionScale);
	Effects::BasicFX->SetDistortionBias(distortionBias);

	return true;
}

void FireShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);
	
	Effects::BasicFX->PassTech->GetPassByIndex(0)->Apply(0, deviceContext);
	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}