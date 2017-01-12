#include "LightClass.h"



LightClass::LightClass(): m_specularPower(0)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(const float red, const float green,const  float blue, const float alpha)
{
	m_ambientColor = Vector4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColor(const float red,const  float green , const float blue, const float alpha)
{
	m_diffuseColor = Vector4(red, green, blue, alpha);
}

void LightClass::SetDirection(const float x,const  float y,const  float z)
{
	m_direction = Vector3(x, y, z);

}

void LightClass::SetSpecularColor(const float red, const float green, const float blue , const float alpha)
{
	m_specularColor = Vector4(red, green, blue, alpha);
	return;
}

void LightClass::SetSpecularPower(const float power) // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	m_specularPower = power;
}

const Vector4& LightClass::GetAmbientColor() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{	
	return m_ambientColor; // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}

const Vector4& LightClass::GetDiffuseColor() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_diffuseColor; // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}

const Vector3& LightClass::GetDirection() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_direction; // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}

const Vector4& LightClass::GetSpecularColor() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_specularColor; // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}

const float& LightClass::GetSpecularPower() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_specularPower; // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}
