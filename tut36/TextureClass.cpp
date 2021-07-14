#include "TextureClass.h"
#include "Macro.hpp"
#include "D3DX11tex.h"
TextureClass::TextureClass()
{
	m_texture1 = nullptr;
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, const CHAR* filename1)
{
	// Load the texture in.
	HR(D3DX11CreateShaderResourceViewFromFile(device, filename1, NULL, NULL, &m_texture1, NULL));
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
	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture1()
{
	return m_texture1;
}