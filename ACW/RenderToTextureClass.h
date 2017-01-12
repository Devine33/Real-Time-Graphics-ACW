#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

class RenderToTextureClass /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{
public:
	RenderToTextureClass();
	RenderToTextureClass(const RenderToTextureClass&) = default;
	RenderToTextureClass& operator=(const RenderToTextureClass&) = default;
	~RenderToTextureClass();

	bool Initialize(ID3D11Device* const, const int, const int, const float screendepth, const float screennear);
	void Shutdown();

	void SetRenderTarget( ID3D11DeviceContext* const ,  ID3D11DepthStencilView* const ) const;
	void ClearRenderTarget(ID3D11DeviceContext* const , ID3D11DepthStencilView* const , const float, const float,const float,const  float) const;
	ID3D11ShaderResourceView* GetShaderResourceView() const;
	void GetProjectionMatrix(Matrix& projectionMatrix) const;
	void GetOrthoMatrix(Matrix& orthoMatrix) const;
	private:
	ID3D11Texture2D*  m_renderTargetTexture;
	ID3D11RenderTargetView*  m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;

	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
	Matrix m_projectionMatrix;
	Matrix m_orthoMatrix;

};

