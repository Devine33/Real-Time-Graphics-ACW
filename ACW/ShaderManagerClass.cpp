#include "ShaderManagerClass.h"



ShaderManagerClass::ShaderManagerClass(): m_Transparency(nullptr), M_Light(nullptr), M_Spotlight(nullptr), m_Texture(nullptr)
{
}


ShaderManagerClass::~ShaderManagerClass()
{
	delete m_Transparency;
	delete M_Light;
	delete M_Spotlight;
	delete m_Texture;
}

bool ShaderManagerClass::Initialize(ID3D11Device*const device, const HWND hwnd)
{
	bool result;
	m_Transparency = new TransparencyShaderClass;
	if (!m_Transparency)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_Transparency->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	M_Light = new TextureShaderClass;
	M_Light->Initialize(device, hwnd);

	M_Spotlight = new SpotLight;
	M_Spotlight->Initialize(device);

	m_Texture = new StandardTextureClass;
	m_Texture->Initialize(device);

	return true;
}

bool ShaderManagerClass::RenderStandardTexture(ID3D11DeviceContext* const devcon, const int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* const texture) const // parasoft-suppress  OPT-24 "Not Included In Spec"
{

	m_Texture->Render(devcon, indexCount, world, view, projection, texture);
	return true;
}

bool ShaderManagerClass::RenderTransparencyShader(ID3D11DeviceContext*const devcon, const int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView*const texture, const float blend) const
{
	bool result;


	// Render the model using the texture shader.
	result = m_Transparency->Render(devcon, indexCount, world, view, projection, texture,blend);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManagerClass::RenderLightShader(ID3D11DeviceContext*const devcon, const int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView*const texture, const Vector3& direction, const Vector4& colour, const Vector4& ambient, const Vector3& cameraposition, const Vector4& specularColor, const float specularPower) const
{
	bool result;

	result = M_Light->Render(devcon, indexCount, world, view, projection, texture, direction,colour,ambient,cameraposition,specularColor,specularPower);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManagerClass::RenderSpotLightShader(ID3D11DeviceContext*const deviceContext, const int indexCount, const Matrix& worldMatrix, const Matrix& viewMatrix, const Matrix& projectionMatrix, ID3D11ShaderResourceView*const texture,  Vector4*const diffuseColor,  Vector4*const lightPosition) const
{
	bool result;
	result = M_Spotlight->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}
	return true;
}

