////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_GroundModel = 0;
	m_WallModel = 0;
	m_BathModel = 0;
	m_WaterModel = 0;
	m_Light = 0;
	m_RefractionTexture = 0;
	m_ReflectionTexture = 0;
	m_LightShader = 0;
	m_RefractionShader = 0;
	m_WaterShader = 0;
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
	
	// Create the ground model object.
	m_GroundModel = new ModelClass;
	if (!m_GroundModel)
	{
		return false;
	}

	// Initialize the ground model object.
	result = m_GroundModel->Initialize(m_D3D->GetDevice(), "ground.txt", "ground01.dds");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the ground model object.", "Error", MB_OK);
		return false;
	}

	// Create the wall model object.
	m_WallModel = new ModelClass;
	if (!m_WallModel)
	{
		return false;
	}

	// Initialize the wall model object.
	result = m_WallModel->Initialize(m_D3D->GetDevice(), "wall.txt", "wall01.dds");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the wall model object.", "Error", MB_OK);
		return false;
	}

	// Create the bath model object.
	m_BathModel = new ModelClass;
	if (!m_BathModel)
	{
		return false;
	}

	// Initialize the bath model object.
	result = m_BathModel->Initialize(m_D3D->GetDevice(), "bath.txt", "marble01.dds");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the bath model object.", "Error", MB_OK);
		return false;
	}

	// Create the water model object.
	m_WaterModel = new ModelClass;
	if (!m_WaterModel)
	{
		return false;
	}

	// Initialize the water model object.
	result = m_WaterModel->Initialize(m_D3D->GetDevice(), "water.txt", "water01.dds");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the water model object.", "Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 0.5f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Create the refraction render to texture object.
	m_RefractionTexture = new RenderTextureClass;
	if (!m_RefractionTexture)
	{
		return false;
	}

	// Initialize the refraction render to texture object.
	result = m_RefractionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the refraction render to texture object.", "Error", MB_OK);
		return false;
	}

	// Create the reflection render to texture object.
	m_ReflectionTexture = new RenderTextureClass;
	if (!m_ReflectionTexture)
	{
		return false;
	}

	// Initialize the reflection render to texture object.
	result = m_ReflectionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the reflection render to texture object.", "Error", MB_OK);
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

	// Create the refraction shader object.
	m_RefractionShader = new RefractionShaderClass;
	if (!m_RefractionShader)
	{
		return false;
	}

	// Initialize the refraction shader object.
	result = m_RefractionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the refraction shader object.", "Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_WaterShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the water shader object.", "Error", MB_OK);
		return false;
	}

	// Set the height of the water.
	m_waterHeight = 2.75f;

	// Initialize the position of the water.
	m_waterTranslation = 0.0f;

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the water shader object.
	if (m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	// Release the refraction shader object.
	if (m_RefractionShader)
	{
		m_RefractionShader->Shutdown();
		delete m_RefractionShader;
		m_RefractionShader = 0;
	}
	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	// Release the reflection render to texture object.
	if (m_ReflectionTexture)
	{
		m_ReflectionTexture->Shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	// Release the refraction render to texture object.
	if (m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
	// Release the water model object.
	if (m_WaterModel)
	{
		m_WaterModel->Shutdown();
		delete m_WaterModel;
		m_WaterModel = 0;
	}

	// Release the bath model object.
	if (m_BathModel)
	{
		m_BathModel->Shutdown();
		delete m_BathModel;
		m_BathModel = 0;
	}

	// Release the wall model object.
	if (m_WallModel)
	{
		m_WallModel->Shutdown();
		delete m_WallModel;
		m_WallModel = 0;
	}

	// Release the ground model object.
	if (m_GroundModel)
	{
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
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
	// Update the position of the water to simulate motion.
	m_waterTranslation += 0.001f;
	if (m_waterTranslation > 1.0f)
	{
		m_waterTranslation -= 1.0f;
	}

	// Set the position and rotation of the camera.
	m_Camera->SetPosition(-10.0f, 6.0f, -10.0f);
	m_Camera->SetRotation(0.0f, 45.0f, 0.0f);
	Render();
	return true;
}


bool GraphicsClass::Render()
{
	bool result;


	// Render the refraction of the scene to a texture.
	result = RenderRefractionToTexture();
	if (!result)
	{
		return false;
	}

	// Render the reflection of the scene to a texture.
	result = RenderReflectionToTexture();
	if (!result)
	{
		return false;
	}

	// Render the scene as normal to the back buffer.
	result = RenderScene();
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::RenderRefractionToTexture() {
	XMFLOAT4 clipPlane;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	// Setup a clipping plane based on the height of the water to clip everything above it.
	clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Translate to where the bath model will be rendered.
	worldMatrix = worldMatrix * XMMatrixTranslation(0.0f, 2.0f, 0.0f);

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_BathModel->Render(m_D3D->GetDeviceContext());

	// Render the bath model using the light shader.
	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_BathModel->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_BathModel->GetTexture(), m_Light->GetDirection(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}


bool GraphicsClass::RenderReflectionToTexture()
{
	XMMATRIX reflectionViewMatrix, worldMatrix, projectionMatrix;
	bool result;


	// Set the render target to be the reflection render to texture.
	m_ReflectionTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the reflection render to texture.
	m_ReflectionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);
	
	// Use the camera to render the reflection and create a reflection view matrix.
	m_Camera->RenderReflection(m_waterHeight);

	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	// Get the world and projection matrices from the d3d object.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Translate to where the wall model will be rendered.
	worldMatrix = worldMatrix * XMMatrixTranslation(0.0f, 6.0f, 8.0f);

	// Put the wall model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_WallModel->Render(m_D3D->GetDeviceContext());

	// Render the wall model using the light shader and the reflection view matrix.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_WallModel->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
		m_WallModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());


	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	
	// Translate to where the ground model will be rendered.
	worldMatrix = worldMatrix * XMMatrixTranslation(0.0f, 1.0f, 0.0f);

	// Put the ground model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_GroundModel->Render(m_D3D->GetDeviceContext());

	// Render the ground model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_GroundModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);
	
	// Translate to where the wall model will be rendered.
	worldMatrix = worldMatrix * XMMatrixTranslation(0.0f, 6.0f, 8.0f);

	// Put the wall model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_WallModel->Render(m_D3D->GetDeviceContext());

	// Render the wall model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_WallModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_WallModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);
	
	// Translate to where the bath model will be rendered.
	worldMatrix = worldMatrix * XMMatrixTranslation(0.0f, 2.0f, 0.0f);

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_BathModel->Render(m_D3D->GetDeviceContext());

	// Render the bath model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_BathModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_BathModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);
	
	// Get the camera reflection view matrix.
	reflectionMatrix = m_Camera->GetReflectionViewMatrix();

	// Translate to where the water model will be rendered.
	worldMatrix = worldMatrix * XMMatrixTranslation(0.0f, m_waterHeight, 0.0f);

	// Put the water model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_WaterModel->Render(m_D3D->GetDeviceContext());

	// Render the water model using the water shader.
	result = m_WaterShader->Render(m_D3D->GetDeviceContext(), m_WaterModel->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, reflectionMatrix, m_ReflectionTexture->GetShaderResourceView(),
		m_RefractionTexture->GetShaderResourceView(), m_WaterModel->GetTexture(),
		m_waterTranslation, 0.01f);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

//bool GraphicsClass::RenderToTexture(float rotation) {
//	XMMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix;
//	bool result;
//	// Set the render target to be the render to texture.
//	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());
//	// Clear the render to texture.
//	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);
//
//	// Use the camera to calculate the reflection matrix.
//	m_Camera->RenderReflection(-1.5f);
//
//	// Get the camera reflection view matrix instead of the normal view matrix.
//	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();
//
//	// Get the world and projection matrices.
//	m_D3D->GetWorldMatrix(worldMatrix);
//	m_D3D->GetProjectionMatrix(projectionMatrix);
//	worldMatrix = XMMatrixRotationY(rotation);
//	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
//	m_Model->Render(m_D3D->GetDeviceContext());
//
//	// Render the model using the texture shader and the reflection view matrix.
//	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
//		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
//		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
//
//	// Reset the render target back to the original back buffer and not the render to texture anymore.
//	m_D3D->SetBackBufferRenderTarget();
//
//	return true;
//}
//
//bool GraphicsClass::RenderScene(float rotation) {
//	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix;
//	bool result;
//
//	// Clear the buffers to begin the scene.
//	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
//	// Generate the view matrix based on the camera's position.
//	m_Camera->Render();
//
//	// Get the world, view, and projection matrices from the camera and d3d objects.
//	m_Camera->GetViewMatrix(viewMatrix);
//	m_D3D->GetWorldMatrix(worldMatrix);
//	m_D3D->GetProjectionMatrix(projectionMatrix);
//
//	// Rotate the world matrix by the rotation value so that the triangle will spin.
//	worldMatrix = XMMatrixRotationY(rotation);
//
//	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
//	m_Model->Render(m_D3D->GetDeviceContext());
//
//	// Render the model using the color shader.
//	// Render the model using the light shader.
//	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
//		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
//		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
//	if (!result)
//	{
//		return false;
//	}
//
//	// Get the world matrix again and translate down for the floor model to render underneath the cube.
//	m_D3D->GetWorldMatrix(worldMatrix);
//	worldMatrix = XMMatrixTranslation(0.0f, -1.5f, 0.0f);
//	// Get the camera reflection view matrix.
//	reflectionMatrix = m_Camera->GetReflectionViewMatrix();
//
//	// Put the floor model vertex and index buffers on the graphics pipeline to prepare them for drawing.
//	m_FloorModel->Render(m_D3D->GetDeviceContext());
//
//	// Render the floor model using the reflection shader, reflection texture, and reflection view matrix.
//	result = m_ReflectionShader->Render(m_D3D->GetDeviceContext(), m_FloorModel->GetIndexCount(), worldMatrix, viewMatrix,
//		projectionMatrix, m_FloorModel->GetTexture(), m_RenderTexture->GetShaderResourceView(),
//		reflectionMatrix);
//
//	// Present the rendered scene to the screen.
//	m_D3D->EndScene();
//
//	return true;
//}