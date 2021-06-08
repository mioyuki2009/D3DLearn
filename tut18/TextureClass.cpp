#include "TextureClass.h"
#include "Macro.hpp"
#include "D3DX11tex.h"
#include <vector>
TextureClass::TextureClass()
{
	m_texture = nullptr;
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const CHAR* filename1, const CHAR* filename2)
{
	// Load the texture in.
	int size = 2;
	std::vector<ID3D11Texture2D*> srcTex(size);
	for (int i = 0; i < 2; i++) {
		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width = D3DX11_FROM_FILE;
		loadInfo.Height = D3DX11_FROM_FILE;
		loadInfo.Depth = D3DX11_FROM_FILE;
		loadInfo.FirstMipLevel = 0;
		loadInfo.MipLevels = D3DX11_FROM_FILE;
		loadInfo.Usage = D3D11_USAGE_STAGING;
		loadInfo.BindFlags = 0;
		loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		loadInfo.MiscFlags = 0;
		loadInfo.Format = DXGI_FORMAT_FROM_FILE;
		loadInfo.Filter = D3DX11_FILTER_NONE;
		loadInfo.MipFilter = D3DX11_FILTER_LINEAR;
		loadInfo.pSrcInfo = 0;
		if (i == 0)
			HR(D3DX11CreateTextureFromFile(device, filename1, &loadInfo, 0, (ID3D11Resource**)&srcTex[i], 0));
		if (i == 1)
			HR(D3DX11CreateTextureFromFile(device, filename2, &loadInfo, 0, (ID3D11Resource**)&srcTex[i], 0));
	}
	
	D3D11_TEXTURE2D_DESC texdesc;
	srcTex[0]->GetDesc(&texdesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texdesc.Width;
	texArrayDesc.Height = texdesc.Height;
	texArrayDesc.MipLevels = texdesc.MipLevels;
	texArrayDesc.ArraySize = size;
	texArrayDesc.Format = texdesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	ID3D11Texture2D* texArray = 0;
	HR(device->CreateTexture2D(&texArrayDesc, 0, &texArray));

	for (UINT texElement = 0; texElement < size; ++texElement)
	{
		// for each mipmap level...
		for (UINT mipLevel = 0; mipLevel < texdesc.MipLevels; ++mipLevel)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			HR(context->Map(srcTex[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D));

			context->UpdateSubresource(texArray,
				D3D11CalcSubresource(mipLevel, texElement, texdesc.MipLevels),
				0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

			context->Unmap(srcTex[texElement], mipLevel);
		}
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = size;

	HR(device->CreateShaderResourceView(texArray, &viewDesc, &m_texture));

	//
	// Cleanup--we only need the resource view.
	//

	ReleaseCOM(texArray);

	for (UINT i = 0; i < size; ++i)
		ReleaseCOM(srcTex[i]);
	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}
