#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include  <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;
#pragma comment(lib, "d3dcompiler.lib")
class TransparencyShaderClass /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{
public:
	TransparencyShaderClass();
	TransparencyShaderClass(const TransparencyShaderClass&) = default;
	TransparencyShaderClass& operator=(const TransparencyShaderClass&) = default;
	~TransparencyShaderClass();
	bool Initialize(ID3D11Device*const, const HWND);
	bool Render(ID3D11DeviceContext*const devcon, const int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView*const texture, const float blend);


private:
	bool InitializeVertexShader(ID3D11Device*const, const HWND, const wchar_t*const, const LPCSTR);
	bool InitializePixelShader(ID3D11Device*const, const wchar_t*const, const LPCSTR);
	bool InitializeShader(ID3D11Device*const, const HWND, const WCHAR*const, const WCHAR*const);
	/*void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);*/
	bool SetShaderParameters(ID3D11DeviceContext*const devcon, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView*const texture, const float);
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
	D3D11_INPUT_ELEMENT_DESC Layout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct TransparentBufferType /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
	{
		float blendAmount;
		Vector3 padding;
	};

	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC transparentBufferDesc;
	MatrixBufferType* dataPtr;
	TransparentBufferType* dataPtr2;

	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_transparentBuffer;
};

