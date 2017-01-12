#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
#include <list>
using namespace DirectX;
using namespace SimpleMath;
class ShadowShaderClass
{
public:
	ShadowShaderClass();
	ShadowShaderClass(const ShadowShaderClass&) = default;
	ShadowShaderClass&operator=(const ShadowShaderClass&) = default;
	~ShadowShaderClass();

	bool Initialize(ID3D11Device*const);
	void Shutdown();
	bool Render(ID3D11DeviceContext*const, const int, const Matrix&, const Matrix&, const Matrix&, const Matrix&, const Matrix&, ID3D11ShaderResourceView*const,
	            ID3D11ShaderResourceView*const, const Vector3&, const Vector4&, const Vector4&) const;

private:
	bool InitializeVertexShader(ID3D11Device*const, const wchar_t*const, const LPCSTR);
	bool InitializePixelShader(ID3D11Device*const, const wchar_t*const, const LPCSTR);
	bool InitializeShader(ID3D11Device*const, const WCHAR*const, const WCHAR*const);

	bool SetShaderParameters(ID3D11DeviceContext*const devcon, const Matrix&, const Matrix&, const Matrix&, const Matrix&, const Matrix&, ID3D11ShaderResourceView*const,
	                         ID3D11ShaderResourceView*const, const Vector3&, const Vector4&, const Vector4&) const;
	void RenderShader(ID3D11DeviceContext*const, const int) const;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleStateWrap;
	ID3D10Blob* pixelShaderBuffer;
	ID3D10Blob* errorMessage;
	ID3D11SamplerState* m_sampleStateClamp;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_lightBuffer2;


	struct MatrixBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
		Matrix lightView;
		Matrix lightProjection;
	};

	struct LightBufferType
	{
		Vector4 ambientColor;
		Vector4 diffuseColor;
	};

	struct LightBufferType2
	{
		Vector3 lightPosition;
		float padding;
	};
};

