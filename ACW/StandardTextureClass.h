#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;
#pragma comment(lib, "d3dcompiler.lib")
class StandardTextureClass
{
public:
	StandardTextureClass();
	StandardTextureClass(const StandardTextureClass&) = default;
	StandardTextureClass&operator=(const StandardTextureClass&) = default;
	~StandardTextureClass();
	bool Initialize(ID3D11Device* const);
	void Shutdown();
	bool Render(ID3D11DeviceContext* const, const int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView* const) const;
private:
	bool InitializeVertexShader(ID3D11Device* const, const wchar_t*const, const LPCSTR);
	bool InitializePixelShader(ID3D11Device* const , const wchar_t*const, const LPCSTR);
	bool InitializeShader(ID3D11Device* const, const WCHAR*const, const WCHAR*const);

	bool SetShaderParameters(ID3D11DeviceContext* const devcon, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* const texture) const;
	void RenderShader(ID3D11DeviceContext* const,const int) const;

	struct ConstantBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};
	ID3D10Blob* errorMessage;
	ID3D10Blob* pixelShaderBuffer;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_constantBuffer;
	ID3D11SamplerState* m_sampleState;
};

