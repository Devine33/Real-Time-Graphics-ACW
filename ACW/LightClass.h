#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;
class LightClass
{
public:
	LightClass();
	~LightClass();

	void SetAmbientColor(const float, const float, const float, const float);
	void SetDiffuseColor(const float,const  float,const  float, const float);
	void SetDirection(const float,const  float,const float);
	void SetSpecularColor(const float, const float, const float,const float);
	void SetSpecularPower(const float);


	const Vector4& GetAmbientColor() const;
	const Vector4& GetDiffuseColor() const;
	const Vector3& GetDirection() const;
	const Vector4& GetSpecularColor() const;
	const float& GetSpecularPower() const;

private:
	Vector4 m_ambientColor;
	Vector4 m_diffuseColor;
	Vector3 m_direction;
	Vector4 m_specularColor;
	float m_specularPower;
};

