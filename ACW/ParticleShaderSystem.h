#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
#include "TextureClass.h"
using namespace DirectX;
using namespace SimpleMath;
class ParticleShaderSystem
{
public:
	ParticleShaderSystem();
	ParticleShaderSystem(const ParticleShaderSystem&) = default;
	ParticleShaderSystem& operator=(const ParticleShaderSystem&) = default;
	~ParticleShaderSystem();

	bool Initialize(ID3D11Device* const);
	void Shutdown();
	bool Render(ID3D11DeviceContext* const,const int,const  Matrix&,const  Matrix&, const Matrix&, ID3D11ShaderResourceView* const) const;

private:
	bool InitializeVertexShader(ID3D11Device* const, const wchar_t*const,const LPCSTR);
	bool InitializePixelShader(ID3D11Device* const, const wchar_t*const,const  LPCSTR);
	bool InitializeShader(ID3D11Device* const,const WCHAR*const, const  WCHAR*const );

	bool SetShaderParameters(ID3D11DeviceContext* const,  const Matrix&, const  Matrix&, const Matrix&, ID3D11ShaderResourceView* const) const;
	void RenderShader(ID3D11DeviceContext* const,const  int) const;

	struct MatrixBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D10Blob* errorMessage;
};

