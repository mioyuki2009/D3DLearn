////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model1 = 0;
	m_Model2 = 0;
	m_LightShader = 0;
}

GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, false, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize Direct3D.", "Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the model object.
	m_Model1 = new ModelClass;
	if(!m_Model1)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model1->Initialize(m_D3D->GetDevice(), "square.txt", "dirt01.dds");
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}

	m_Model2 = new ModelClass;
	if (!m_Model2)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model2->Initialize(m_D3D->GetDevice(), "square.txt", "stone01.dds");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}
	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the light shader object.", "Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_Model1)
	{
		m_Model1->Shutdown();
		delete m_Model1;
		m_Model1 = 0;
	}

	if (m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;


	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)0.01f;
	if (rotation > XM_PI)
	{
		rotation -= XM_PI;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Setup a clipping plane.
	XMFLOAT4 clipPlane = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	// Set the blending amount to 50%.
	float blendAmount = 0.5f;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	// Clear the buffers to begin the scene.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model1->Render(m_D3D->GetDeviceContext());

	// Render the model with the clip plane shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model1->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_Model1->GetTexture1(), clipPlane, 1.0f);
	if (!result)
	{
		return false;
	}

	// Translate to the right by one unit and towards the camera by one unit.
	worldMatrix = worldMatrix * XMMatrixTranslation(1.0f, 0.0f, -1.0f);
	// Turn on alpha blending for the transparency to work.
	m_D3D->TurnOnAlphaBlending();

	// Put the second square model on the graphics pipeline.
	m_Model2->Render(m_D3D->GetDeviceContext());
	clipPlane =  XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	// Render the second square model with the stone texture and use the 50% blending amount for transparency.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_Model2->GetTexture1(), clipPlane, blendAmount);
	if (!result)
	{
		return false;
	}
	// Turn off alpha blending.
	m_D3D->TurnOffAlphaBlending();
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::RenderToTexture(float rotation) {
	return true;
}

bool GraphicsClass::RenderScene(float rotation) {

	return true;
}