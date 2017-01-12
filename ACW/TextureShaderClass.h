#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include  <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;
#pragma comment(lib, "d3dcompiler.lib")
class TextureShaderClass /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{
public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&) = default;
	TextureShaderClass& operator=(const TextureShaderClass&) = default;

	~TextureShaderClass();
	bool Initialize(ID3D11Device*const, const HWND);
	bool Render(ID3D11DeviceContext*const, const int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*const, const Vector3&, const Vector4&, const Vector4&, const Vector3&, const Vector4&, const float);
private:
	bool InitializeVertexShader(ID3D11Device*const, const HWND, const wchar_t*const, const LPCSTR);
	bool InitializePixelShader(ID3D11Device*const, const wchar_t*const, const LPCSTR);
	bool InitializeShader(ID3D11Device*const, const HWND, const WCHAR*const, const WCHAR*const);
	/*void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);*/
	bool SetShaderParameters(ID3D11DeviceContext*const devcon, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView*const texture, const Vector3&, const Vector4&, const Vector4&, const Vector3&, const Vector4&, const float);
	void RenderShader(ID3D11DeviceContext*const, const int) const;


	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;

	ID3DBlob* m_blob = nullptr;
	ID3D11SamplerState* m_sampleState;
	//Init Shader

	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC Layout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct CameraBufferType
	{
		Vector3 cameraPosition;
		float padding;
	};

	struct LightBufferType /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
	{
		Vector4 ambientColor;
		Vector4 diffuseColor;
		Vector3 lightDirection;
		float specularPower;
		Vector4 specularColor;
	};

	D3D11_SAMPLER_DESC samplerDesc;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	CameraBufferType* dataPtr3;

	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;
};

