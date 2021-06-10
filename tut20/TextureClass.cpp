#include "TextureClass.h"
#include "Macro.hpp"
#include "D3DX11tex.h"
TextureClass::TextureClass()
{
	m_texture1 = nullptr;
	m_texture2 = nullptr;
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, const CHAR* filename1, const CHAR* filename2)
{
	// Load the texture in.
	HR(D3DX11CreateShaderResourceViewFromFile(device, filename1, NULL, NULL, &m_texture1, NULL));
	HR(D3DX11CreateShaderResourceViewFromFile(device, filename2, NULL, NULL, &m_texture2, NULL));
	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture resource.
	if (m_texture1)
	{
		m_texture1->Release();
		m_texture1 = 0;
	}
	if (m_texture2)
	{
		m_texture2->Release();
		m_texture2 = 0;
	}
	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture1()
{
	return m_texture1;
}
ID3D11ShaderResourceView* TextureClass::GetTexture2()
{
	return m_texture2;
}
