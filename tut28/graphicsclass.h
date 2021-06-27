////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "Bitmapclass.h"
#include "FadeShaderClass.h"
#include "RenderTextureClass.h"
/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


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
	bool Frame(float);
private:
	bool Render(float);
	bool RenderToTexture(float);

	bool RenderNormalScene(float);
	bool RenderFadingScene();
private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	RenderTextureClass* m_RenderTexture;

	BitmapClass* m_Bitmap;
	float m_fadeInTime, m_accumulatedTime, m_fadePercentage;
	bool m_fadeDone;
	FadeShaderClass* m_FadeShader;

};
