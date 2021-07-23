////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "rendertextureclass.h"
#include "orthowindowclass.h"
#include "horizontalblurshaderclass.h"
#include "verticalblurshaderclass.h"
#include "glowmapshaderclass.h"
#include "glowshaderclass.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();
	bool RenderGlowMapToTexture();
	bool DownSampleTexture();
	bool RenderHorizontalBlurToTexture();
	bool RenderVerticalBlurToTexture();
	bool UpSampleTexture();
	bool RenderUIElementsToTexture();
	bool RenderGlowScene();
private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	RenderTextureClass* m_RenderTexture, * m_DownSampleTexure, * m_HorizontalBlurTexture, * m_VerticalBlurTexture, * m_UpSampleTexure;
	OrthoWindowClass* m_SmallWindow, * m_FullScreenWindow;
	HorizontalBlurShaderClass* m_HorizontalBlurShader;
	VerticalBlurShaderClass* m_VerticalBlurShader;
	GlowMapShaderClass* m_GlowMapShader;
	GlowShaderClass* m_GlowShader;
	
};
