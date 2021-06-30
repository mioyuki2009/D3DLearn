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
#include "rendertextureclass.h"
#include "refractionshaderclass.h"
#include "watershaderclass.h"
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
	bool Frame();
	bool Render();

private:
	
	bool RenderRefractionToTexture();
	bool RenderReflectionToTexture();
	bool RenderScene();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	ModelClass* m_GroundModel, * m_WallModel, * m_BathModel, * m_WaterModel;

	RenderTextureClass* m_RefractionTexture, * m_ReflectionTexture;
	RefractionShaderClass* m_RefractionShader;
	WaterShaderClass* m_WaterShader;

	float m_waterHeight, m_waterTranslation;
};