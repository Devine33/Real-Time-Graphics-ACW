#pragma once
//Partially referenced from rastertek
//was used to create a basic camera then adapted to use rotations and external movements 16-11-2016
#include <d3d11.h>
#include <directxmath.h>
#include <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;
class CameraClass /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{
public:
	CameraClass();
	~CameraClass();
	// Create camera, based on another one
	CameraClass(const CameraClass& camera) = default;

	// Copy all camera's parameters
	CameraClass& operator=(const CameraClass& camera) = default;


	void SetPosition( const float x,const  float y,const  float z);
	void SetRotation(const float x,const  float y,const  float z);
	void cameraSetup(const float screenheight, const float screenwidth);
	void Render();
	void GetViewMatrix(Matrix&) const;
	void GetProjectionMatrix(Matrix&) const;
	void GetWorldMatrix(Matrix&) const;
	Vector3 GetPosition() const;
	Vector3 GetRotation() const;
	void Position1();
	void Position2();
	void IncrementZAxis();
	void DecrementZAxis();


	void PitchPositive();
	void YawPositive();
	void RollPositive();

	void PitchNegative();
	void YawNegative();
	void RollNegative();

	void IncrementYAxis();
	void DecrementYAxis();
	void IncrementXAxis();
	void DecrementXAxis();
private:
	float screenaspect;
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	SimpleMath::Matrix m_projectionMatrix;
	SimpleMath::Matrix m_worldMatrix;
	SimpleMath::Matrix m_orthoMatrix;
	Matrix m_viewMatrix;
	float screenHeight;
	float ScreenWidth;
	Vector3 up, position, lookAt;
	float yaw, pitch, roll;
	Matrix rotationMatrix;
	
	float radians;
};

