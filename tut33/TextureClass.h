#pragma once
#include <d3d11.h>

class TextureClass
{
public:
	TextureClass();
	~TextureClass();

	bool Initialize(ID3D11Device*,const CHAR*, const CHAR*, const CHAR*);
	void Shutdown();
	
	ID3D11ShaderResourceView* GetTexture1();
	ID3D11ShaderResourceView* GetTexture2();
	ID3D11ShaderResourceView* GetTexture3();
private:
	ID3D11ShaderResourceView* m_texture1;
	ID3D11ShaderResourceView* m_texture2;
	ID3D11ShaderResourceView* m_texture3;
};