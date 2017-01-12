#include "ShadowLightClass.h"



ShadowLightClass::ShadowLightClass()
{
}


ShadowLightClass::~ShadowLightClass()
{
}

void ShadowLightClass::SetAmbientColor(const float red, const float green, const float blue, const float alpha)
{
	m_ambientColor = Vector4(red, green, blue, alpha);
	return;
}

void ShadowLightClass::SetDiffuseColor(const float red, const float green, const float blue, const float alpha)
{
	m_diffuseColor = Vector4(red, green, blue, alpha);
	return;
}

void ShadowLightClass::SetPosition(const float x, const float y, const float z)
{
	m_position = Vector3(x, y, z);
}

void ShadowLightClass::SetLookAt(const float x, const float y, const float z) // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	m_lookAt.x = x;
	m_lookAt.y = y;
	m_lookAt.z = z;
	return;
}

const Vector4& ShadowLightClass::GetAmbientColor() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_ambientColor; // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}

const Vector4& ShadowLightClass::GetDiffuseColor() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_diffuseColor; // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}

const Vector3& ShadowLightClass::GetPosition() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_position; // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}

void ShadowLightClass::GenerateViewMatrix() 
{
	Vector3 up;
	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	
	m_viewMatrix = XMMatrixLookAtLH(m_position, m_lookAt, up);
	// Create the view matrix from the three vectors.
	 
}

void ShadowLightClass::GenerateProjectionMatrix(const float screenDepth,const float screenNear) 
{
	float fieldOfView, screenAspect;
	// Setup field of view and screen aspect for a square light source.
	fieldOfView = XM_PI / 2.0f;
	screenAspect = 1.0f;
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	// Create the projection matrix for the light.

}

void ShadowLightClass::GetViewMatrix(Matrix& viewMatrix) const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	viewMatrix = m_viewMatrix;
	return;
}

void ShadowLightClass::GetProjectionMatrix(Matrix& projectionMatrix) const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	projectionMatrix = m_projectionMatrix;
	return;
}
