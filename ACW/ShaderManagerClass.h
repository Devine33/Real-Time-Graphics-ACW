#pragma once
#include "DirectXClass.h"
#include  "TextureShaderClass.h"
#include "TransparencyShaderClass.h"
#include "SpotLight.h"
#include "StandardTextureClass.h"

class ShaderManagerClass
{
	//TextureShader
	//LightTextureShader
	//TransparentShader
	//
public:
	ShaderManagerClass();
	ShaderManagerClass(ShaderManagerClass&) = default;

	~ShaderManagerClass();
	bool Initialize(ID3D11Device*const device, const HWND hwnd);
	ShaderManagerClass& operator=(const ShaderManagerClass&) = default;
	void Shutdown();
	bool RenderStandardTexture(ID3D11DeviceContext* const devcon, const int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* const texture) const;
	bool RenderTransparencyShader(ID3D11DeviceContext*const devcon, const int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView*const texture, const float blend) const;
	bool RenderLightShader(ID3D11DeviceContext*const devcon, const int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView*const texture, const Vector3& direction, const Vector4& colour, const Vector4& ambient, const Vector3& cameraposition, const Vector4& specularColor, const float specularPower) const;
	bool RenderSpotLightShader(ID3D11DeviceContext*const deviceContext, const int indexCount, const Matrix& worldMatrix, const Matrix& viewMatrix, const Matrix& projectionMatrix, ID3D11ShaderResourceView*const texture,  Vector4*const diffuseColor,  Vector4*const lightPosition) const;

private:
	TransparencyShaderClass* m_Transparency;
	TextureShaderClass* M_Light;
	SpotLight* M_Spotlight;
	StandardTextureClass* m_Texture;
};

