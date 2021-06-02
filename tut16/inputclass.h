////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	enum MouseStateChange
	{
		NONE,
		LUP,
		LDOWN,
		RUP,
		RDOWN
	};
public:
	struct MouseState
	{
		bool LbuttonState;
		bool RbuttonState;
	};

	InputClass();
	~InputClass();

	bool Initialize(int, int);

	void Shutdown();
	bool Frame(int, int, MouseStateChange);

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();

private:
	bool ReadKeyboard();
	bool ReadMouse(int, int, MouseStateChange);
	void ProcessInput();

private:

	unsigned char m_keyboardState[256];
	
	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	MouseState m_mouseState;
};

#endif