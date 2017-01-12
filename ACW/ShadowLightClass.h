#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;
class ShadowLightClass
{
public:
	ShadowLightClass();
	~ShadowLightClass();

	void SetAmbientColor(const float, const float, const float, const float);
	void SetDiffuseColor(const float, const float, const float, const float);
	void SetPosition(const float, const float, const float);
	void SetLookAt(const float, const float, const float);

	const Vector4& GetAmbientColor() const;
	const Vector4& GetDiffuseColor() const;
	const Vector3& GetPosition() const;

	void GenerateViewMatrix() ;
	void GenerateProjectionMatrix(const float,const  float) ;

	void GetViewMatrix(Matrix&) const;
	void GetProjectionMatrix(Matrix&) const;

private:
	Vector4 m_ambientColor;
	Vector4 m_diffuseColor;
	Vector3 m_position;
	Vector3 m_lookAt;
	Matrix m_viewMatrix;
	Matrix m_projectionMatrix;
};

