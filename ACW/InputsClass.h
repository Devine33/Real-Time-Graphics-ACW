#pragma once
class InputsClass
{
public:
	InputsClass();
	InputsClass(const InputsClass&);
	~InputsClass();

	void Initialize();

	void KeyDown(const unsigned int);
	void KeyUp(const unsigned int);

	bool IsKeyDown(const unsigned int) const;

private:
	bool m_keys[256];

};

