////////////////////////////////////////////////////////////////////////////////
// Filename: colorshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightshaderclass.h"
#include "Effects.h"
LightShaderClass::LightShaderClass()
{
	m_layout = 0;
}

LightShaderClass::~LightShaderClass()
{
}


bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
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

void LightShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, 
	XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, ambientColor, diffuseColor, 
		cameraPosition, specularColor, specularPower);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
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
	
	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	D3DX11_PASS_DESC passDesc;
	Effects::BasicFX->PassTech1->GetPassByIndex(0)->GetDesc(&passDesc);
	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void LightShaderClass::ShutdownShader()
{
	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}
	return;
}

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection,
	XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, 
	XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower)
{
	Effects::BasicFX->SetWorldMatrix(worldMatrix);
	Effects::BasicFX->SetViewMatrix(viewMatrix);
	Effects::BasicFX->SetProjMatrix(projectionMatrix);
	Effects::BasicFX->SetTex(texture);

	XMVECTOR aColor = XMLoadFloat4(&ambientColor);
	XMVECTOR dColor = XMLoadFloat4(&diffuseColor);
	Effects::BasicFX->SetLightAmbient(aColor);
	Effects::BasicFX->SetLightColor(dColor);
	Effects::BasicFX->SetLightDirection(lightDirection);

	XMVECTOR sColor = XMLoadFloat4(&specularColor);
	Effects::BasicFX->SetSpecularColor(sColor);
	Effects::BasicFX->SetSpecularPower(specularPower);
	Effects::BasicFX->SetCameraPosition(cameraPosition);
	return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);
	
	Effects::BasicFX->PassTech1->GetPassByIndex(0)->Apply(0, deviceContext);
	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}