#include "CameraClass.h"


CameraClass::CameraClass(): screenaspect(0), m_positionX(0), m_positionY(0), m_positionZ(0), m_rotationX(0), m_rotationY(0), m_rotationZ(0), screenHeight(0), ScreenWidth(0), yaw(0), pitch(0), roll(0), radians(0)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(const float x, const  float y, const  float z) // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void CameraClass::SetRotation(const float x, const  float y, const  float z) // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

void CameraClass::cameraSetup(const float screenheight, const float screenwidth)
{
	screenaspect = screenheight / screenwidth;
	screenHeight = screenheight;
	ScreenWidth = screenwidth;
}

void CameraClass::Render()
{

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;


	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	radians = m_rotationY * 0.0174532925f;

	// Setup where the camera is looking by default.
	lookAt.x = sinf(radians) + m_positionX;
	lookAt.y = m_positionY;
	lookAt.z = 100.0f;//cosf(radians) + m_positionZ;

	//1.0f

	//////// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	////// Create the rotation matrix from the yaw, pitch, and roll values.
	//
	rotationMatrix =  Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);
	//// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	float fieldOfView;
	// Translate the rotated camera position to the location of the viewer.
	
	fieldOfView = static_cast<float>(XM_PI) / 4.0f;
	/*screenaspect = height / width;*/
	// Create the projection matrix for 3D rendering.
	m_worldMatrix = XMMatrixIdentity();


	m_orthoMatrix = XMMatrixOrthographicLH(ScreenWidth, screenHeight, 1.0f, 1000.0f);

	m_viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenaspect, 1.0f, 1000.0F);
}

void CameraClass::GetViewMatrix(Matrix& viewMatrix) const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	viewMatrix = m_viewMatrix;
}

void CameraClass::GetProjectionMatrix(Matrix& projection) const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	projection = m_projectionMatrix;
}

void CameraClass::GetWorldMatrix(Matrix& world) const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	world = m_worldMatrix;
}

Vector3 CameraClass::GetPosition() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	return Vector3(m_positionX, m_positionY, m_positionZ);
}

Vector3 CameraClass::GetRotation() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	return Vector3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::IncrementZAxis()
{
	m_positionZ += 0.4;
}

void CameraClass::DecrementZAxis()
{
	m_positionZ -= 0.4;
}

void CameraClass::PitchNegative()
{
	m_rotationX += 0.4f;
}

void CameraClass::YawPositive()
{
	m_rotationY += 0.4f;
}

void CameraClass::RollPositive()
{
	m_rotationZ += 0.4f;
}

void CameraClass::PitchPositive()
{
	m_rotationX -= 0.4f;
}

void CameraClass::YawNegative()
{
	m_rotationY -= 0.4f;
}

void CameraClass::RollNegative()
{
	m_positionZ -= 0.4f;
}

void CameraClass::IncrementYAxis()
{
	m_positionY += 0.4;
}

void CameraClass::Position1() // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f,0.0f,0.0f);
}

void CameraClass::Position2()
{
	SetPosition(-20.0f, -10.0f, 0.0f);
	SetRotation(10.0f, 30.0f, 0.0f);
}

void CameraClass::DecrementYAxis()
{
	m_positionY -= 0.1;
}

void CameraClass::IncrementXAxis()
{
	m_positionX += 0.1;
}
void CameraClass::DecrementXAxis()
{
	m_positionX -= 0.1;
}



