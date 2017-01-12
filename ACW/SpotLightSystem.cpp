#include "SpotLightSystem.h"



SpotLightSystem::SpotLightSystem()
{
}


SpotLightSystem::~SpotLightSystem()
{
}

void SpotLightSystem::SetDiffuseColor(const float red, const float green, const float blue, const float alpha)
{
	m_diffuseColor = Vector4(red, green, blue, alpha);
	return;
}

void SpotLightSystem::SetPosition(const float x, const float y, const float z)
{
	m_position = Vector4(x, y, z, 1.0f);
}

const Vector4& SpotLightSystem::GetDiffuseColor() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_diffuseColor; // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}

const Vector4& SpotLightSystem::GetPosition() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_position; // parasoft-suppress  CODSTA-CPP-06-DOWNGRADED "Not Included In Spec"
}
