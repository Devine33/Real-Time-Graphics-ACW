#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
using namespace std;
using namespace DirectX;
using namespace SimpleMath;
class DepthShaderClass
{
public:
	DepthShaderClass();
	DepthShaderClass(const DepthShaderClass&) = default;
	DepthShaderClass& operator=(const DepthShaderClass&) = default;
	~DepthShaderClass();
	bool Initialize(ID3D11Device* const);
	void Shutdown();
	bool Render(ID3D11DeviceContext* const ,const int, const Matrix&,const Matrix&, const Matrix&) const;


private:
	bool InitializeShader(ID3D11Device* const, const WCHAR* const, const WCHAR* const );
	bool SetShaderParameters(ID3D11DeviceContext* const, const Matrix&, const Matrix&, const Matrix&) const;
	void RenderShader(ID3D11DeviceContext* const, const int) const;

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



};

