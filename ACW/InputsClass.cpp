#include "InputsClass.h"



InputsClass::InputsClass(): m_keys{true}
{
}

InputsClass::InputsClass(const InputsClass&) : m_keys{ true }
{
}

InputsClass::~InputsClass()
{
}

void InputsClass::Initialize()
{
	// Initialize all the keys to being released and not pressed.
	for (int i = 0; i<256; i++)
	{
		m_keys[i] = false;
	}
}

void InputsClass::KeyDown(const unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
}

void InputsClass::KeyUp(const unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
}

bool InputsClass::IsKeyDown(const unsigned int key) const
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}
