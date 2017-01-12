#pragma once
//referenced from rastertek to load bump map shaders
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;
class BumpMapShaderClass
{
public:
	BumpMapShaderClass();
	BumpMapShaderClass(const BumpMapShaderClass&) = default;
	BumpMapShaderClass& operator=(const BumpMapShaderClass&)  = default;
	~BumpMapShaderClass();

	bool Initialize(ID3D11Device*const device);
	void Shutdown();
	bool Render(ID3D11DeviceContext*const devcon, const int indexCount, const Matrix& world,const Matrix& view,const Matrix& projection, ID3D11ShaderResourceView**const textureArray,const Vector3& lightDirection,const Vector4& DiffuseColor) const;
private:

	bool InitializeVertexShader(ID3D11Device*const  device, const wchar_t*const  filename, const LPCSTR entrypoint);
	bool InitializePixelShader(ID3D11Device*const device, const wchar_t*const filename,const  LPCSTR entrypoint);
	bool InitializeShader(ID3D11Device*const  device,const WCHAR*const, const WCHAR*const);


	bool SetShaderParameters(ID3D11DeviceContext*const devcon,const Matrix& world,const Matrix& view,const Matrix& projection, ID3D11ShaderResourceView**const textureArray,const Vector3& lightDirection,const Vector4& DiffuseColor) const;
	void RenderShader(ID3D11DeviceContext*const devcon, const int indexCount) const;

	struct MatrixBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

	struct LightBufferType
	{
		Vector4 diffuseColor;
		Vector3 lightDirection;
		float padding;
	};
	
	ID3D10Blob* errorMessage;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;

	ID3D11Buffer* m_lightBuffer;

};

