#pragma once
#include <d3d11.h>

class TextureClass
{
public:
	TextureClass();
	~TextureClass();

	bool Initialize(ID3D11Device*,const CHAR*);
	void Shutdown();
	
	ID3D11ShaderResourceView* GetTexture();

private:
	
	ID3D11ShaderResourceView* m_texture;
};