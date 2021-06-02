////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"
#include "windows.h"

InputClass::InputClass()
{
	m_mouseX = 0;
	m_mouseY = 0;

	m_mouseState.LbuttonState = false;
	m_mouseState.RbuttonState = false;

	memset(m_keyboardState, 0, sizeof(char) * 256);
}

bool InputClass::IsLeftArrowPressed() {
	if (m_keyboardState[VK_LEFT] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsRightArrowPressed() {
	if (m_keyboardState[VK_RIGHT] & 0x80)
	{
		return true;
	}
	return false;
}

InputClass::~InputClass()
{
}


bool InputClass::Initialize(int screenWidth, int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0;
	m_mouseY = 0;

	return true;
}

void InputClass::Shutdown()
{

	return;
}

bool InputClass::Frame(int x, int y, MouseStateChange mouseState)
{
	bool result;

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse(x, y, mouseState);
	if (!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard()
{
	bool result;

	// Read the keyboard device.
	result = GetKeyboardState(m_keyboardState);

	return result;
}

bool InputClass::ReadMouse(int x, int y, MouseStateChange mouseState)
{
	if (x < 0 || y < 0)
		return true;
	m_mouseX = x;
	m_mouseY = y;
	switch (mouseState)
	{
	case InputClass::LUP:
		m_mouseState.LbuttonState = false;
		break;
	case InputClass::LDOWN:
		m_mouseState.LbuttonState = true;
		break;
	case InputClass::RUP:
		m_mouseState.RbuttonState = false;
		break;
	case InputClass::RDOWN:
		m_mouseState.RbuttonState = true;
		break;
	default:
		break;
	}
	return true;
}

void InputClass::ProcessInput()
{

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}

bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[VK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}
