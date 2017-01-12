#include "SystemsClass.h"
#include <AntTweakBar.h>
#include <winapifamily.h>


SystemsClass::SystemsClass(): m_applicationName(nullptr), m_hinstance(nullptr), m_hwnd(nullptr), Camera(nullptr), m_Input(nullptr), m_Graphics(nullptr)
{
}

SystemsClass::~SystemsClass()
{
	delete m_Input;
	delete m_Graphics;
}

bool SystemsClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputsClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(m_hwnd,screenWidth,screenHeight);
	if (!result)
	{
		return false;
	}

	return true;

}

void SystemsClass::Shutdown() const
{
}

void SystemsClass::Run() const
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = false;
			}
		}

	}
}

LRESULT SystemsClass::MessageHandler(const  HWND hwnd,const  UINT umsg,const  WPARAM wparam, const LPARAM lparam) const
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		m_Input->KeyDown(static_cast<unsigned int>(wparam));
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		m_Input->KeyUp(static_cast<unsigned int>(wparam));
		return 0;
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

bool SystemsClass::Frame() const
{
	bool result;


	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		PostQuitMessage(0);
		return false;
	}
	if(m_Input->IsKeyDown(87))
	{
		m_Graphics->IncrementZ();
	}
	if (m_Input->IsKeyDown(83))
	{
		m_Graphics->DecrementZ();
	}
	if (m_Input->IsKeyDown(65))
	{
		m_Graphics->IncrementY();
	}
	if (m_Input->IsKeyDown(68))
	{
		m_Graphics->DecrementY();
	}
	if (m_Input->IsKeyDown(88))
	{
		m_Graphics->IncrementX();
	}
	if (m_Input->IsKeyDown(90))
	{
		m_Graphics->DecrementX();
	}
	//right arrow
	if (m_Input->IsKeyDown(39))
	{
		m_Graphics->YawPositive();
	}
	//up arrow
	if (m_Input->IsKeyDown(38))
	{
		m_Graphics->PitchPositive();
	}
	//37 left arrow
	if (m_Input->IsKeyDown(37))
	{
		m_Graphics->YawNegative();
	}
	// down arrow
	if (m_Input->IsKeyDown(40))
	{
		m_Graphics->PitchNegative();
	}
	if(m_Input->IsKeyDown(VK_F5))
	{
	m_Graphics->SetWireFrame();
	
	}
	if (m_Input->IsKeyDown(VK_F1))
	{
		m_Graphics->Position1();
	}
	if (m_Input->IsKeyDown(VK_F2))
	{
		m_Graphics->Position2();
	}


	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemsClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandleW(nullptr);

	// Give the application a name.
	m_applicationName = L"Engine";
	// Register the window class.

	_WND.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	_WND.lpfnWndProc	= WindowProc;
	_WND.cbClsExtra		= 0;
	_WND.cbWndExtra		= 0;
	_WND.hInstance		= m_hinstance;
	_WND.hIcon			= LoadIcon(nullptr, IDI_WINLOGO);
	_WND.hIconSm = _WND.hIcon;
	_WND.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	_WND.hbrBackground	= static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	_WND.lpszMenuName	= nullptr;
	_WND.lpszClassName	= m_applicationName;
	_WND.cbSize = sizeof(WNDCLASSEX);


	RegisterClassEx(&_WND);

	//// Determine the resolution of the clients desktop screen.
	//screenWidth = 1000;//GetSystemMetrics(SM_CXSCREEN);
	//screenHeight = 1000;//GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN == true)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(screenWidth);
		dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(screenHeight);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(0, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_OVERLAPPEDWINDOW,
		posX, posY, screenWidth, screenHeight, nullptr, nullptr, m_hinstance, nullptr);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);
}

LRESULT CALLBACK  WindowProc(const HWND hwnd, const UINT message, const WPARAM wParam,const  LPARAM lParam)
{
	if (TwEventWin(hwnd, message, wParam, lParam))
		return 0;
	switch (message)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	//// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, message, wParam, lParam);
	}
	}
}
