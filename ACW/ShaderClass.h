#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;
#pragma comment(lib, "d3dcompiler.lib")
class ShaderClass /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{
public:
	ShaderClass();
	ShaderClass(const ShaderClass&);
	ShaderClass& operator=(const ShaderClass&) = default;
	~ShaderClass();
	bool Initialize(ID3D11Device*const dev);
	bool Render(ID3D11DeviceContext*const,const int, const XMMATRIX&,const XMMATRIX&, const XMMATRIX&);

private:
	bool InitializeVertexShader(ID3D11Device*const, const wchar_t*const, const LPCSTR);
	bool InitializePixelShader(ID3D11Device*const , const wchar_t*const ,const LPCSTR);
	//void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext*const,const XMMATRIX&,const XMMATRIX&,const XMMATRIX&);
	void RenderShader(ID3D11DeviceContext*const,const int) const;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3DBlob* m_blob = nullptr;

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
	MatrixBufferType* dataPtr;
};

