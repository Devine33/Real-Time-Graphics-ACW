#pragma once
#include "GraphicsClass.h"
#include "InputsClass.h"
#include "CameraClass.h"
#define WIN32_LEAN_AND_MEAN
class SystemsClass /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{
public:
	SystemsClass();
	SystemsClass(const SystemsClass&) = default;
	SystemsClass& operator=(const SystemsClass&) = default;
	~SystemsClass();

	bool Initialize();
	void Shutdown() const;
	void Run() const;
	LRESULT CALLBACK MessageHandler(const HWND, const UINT,const  WPARAM, const LPARAM) const;
private:
	bool Frame() const;
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	WNDCLASSEX _WND;
	RECT rc;

	CameraClass *Camera;
	InputsClass* m_Input;
	GraphicsClass* m_Graphics;
};


 LRESULT CALLBACK WindowProc(const HWND hwnd,const UINT message,const WPARAM wParam,const LPARAM lParam);
 static SystemsClass* ApplicationHandle = nullptr;
