#pragma once

#include <d3d11.h>
#include <xnamath.h>
#include "TextureClass.h"

class FontClass
{
private:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	struct FontType
	{
		float left, right;
		int size;
	};
public:
	FontClass();
	~FontClass();

	bool Initialize(ID3D11Device*, const char*, const char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	void BuildVertexArray(void*, const char*, float, float);

private:
	bool LoadFontData(const char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, const char*);
	void ReleaseTexture();

private:
	FontType* m_Font;
	TextureClass* m_Texture;
};

