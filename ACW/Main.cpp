#include "SystemsClass.h"

int WINAPI WinMain(HINSTANCE,
	HINSTANCE,
	LPSTR,
	int)
{
	SystemsClass* System;
	bool result;


	// Create the system object.
	System = new SystemsClass;
	if (!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}
