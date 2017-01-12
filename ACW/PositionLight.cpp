#include "PositionLight.h"



PositionLight::PositionLight()
{
}


PositionLight::~PositionLight()
{
}

void PositionLight::SetAmbientColor(const float red, const float green, const float blue, const float alpha)
{
	m_ambientColor = Vector4(red, green, blue, alpha);
}

void PositionLight::SetDiffuseColor(const float red, const float green, const float blue, const float alpha)
{
	m_diffuseColor = Vector4(red, green, blue, alpha);
}

void PositionLight::SetPosition(const float x, const float y, const float z)
{
	m_position = Vector3(x, y, z);
	return;
}

void PositionLight::SetLookAt(const float x, const float y, const float z)
{
	m_lookAt = Vector3(x, y, z);
}

const Vector4& PositionLight::GetAmbientColor() const  // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_ambientColor;  // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}

const Vector4& PositionLight::GetDiffuseColor() const  // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_diffuseColor;  // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}

const Vector3& PositionLight::GetPosition() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_position;  // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}

void PositionLight::GenerateViewMatrix()
{
	Vector3 up;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Create the view matrix from the three vectors.
	m_viewMatrix = Matrix::CreateLookAt(m_position, m_lookAt, up);


}

void PositionLight::GenerateProjectionMatrix(const float screenDepth, const float screenNear)
{
	float fieldOfView, screenAspect;

	fieldOfView = static_cast<float>(XM_PI) / 2.0f;
	screenAspect = 1.0f;

	// Create the projection matrix for the light.
	m_projectionMatrix = Matrix::CreatePerspectiveFieldOfView(fieldOfView, screenAspect, screenNear,screenDepth);
}


void PositionLight::GetViewMatrix(Matrix& viewMatrix) const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	viewMatrix = m_viewMatrix;
	return;
}

void PositionLight::GetProjectionMatrix(Matrix& projectionMatrix) const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	projectionMatrix = m_projectionMatrix;
}
