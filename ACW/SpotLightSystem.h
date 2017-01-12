#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;
class SpotLightSystem
{
public:
	SpotLightSystem();
	~SpotLightSystem();

	void SetDiffuseColor(const float red, const float green, const float blue, const float alpha);
	void SetPosition(const float, const float, const float);

	const Vector4& GetDiffuseColor() const;
	const Vector4& GetPosition() const;
private:
	Vector4 m_diffuseColor;
	Vector4 m_position;
};

