#pragma once
/////////////
// GLOBALS //
/////////////
const int NUM_LIGHTS = 4;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;
class SpotLight
{
public:
	SpotLight();
	SpotLight(const SpotLight&) = default;
	SpotLight&operator=(const SpotLight&) = default;
	~SpotLight();
	bool Initialize(ID3D11Device*const device);
	void Shutdown();
	bool Render(ID3D11DeviceContext*const deviceContext, const int indexCount, const Matrix& worldMatrix, const Matrix& viewMatrix,
	            const Matrix& projectionMatrix, ID3D11ShaderResourceView*const texture, const Vector4*const diffuseColor,
	            const Vector4*const lightPosition) const;


private:
	bool InitializeShader(ID3D11Device*const, const WCHAR*const, const WCHAR*const);
	bool InitializeVertexShader(ID3D11Device*const, const wchar_t*const, const LPCSTR);
	bool InitializePixelShader(ID3D11Device*const, const wchar_t*const, const LPCSTR);
	bool SetShaderParameters(ID3D11DeviceContext*const, const Matrix&, const Matrix&, const Matrix&, ID3D11ShaderResourceView*const, const Vector4*const, const Vector4*const) const;
	void RenderShader(ID3D11DeviceContext*const, const int) const;

	struct MatrixBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};
	struct LightColorBufferType
	{
		Vector4 diffuseColor[NUM_LIGHTS];
	};

	struct LightPositionBufferType
	{
		Vector4 lightPosition[NUM_LIGHTS];
	};

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightColorBuffer;
	ID3D11Buffer* m_lightPositionBuffer;
	ID3D10Blob* pixelShaderBuffer;
	ID3D10Blob* errorMessage;
};

