#include "RenderToTextureClass.h"



RenderToTextureClass::RenderToTextureClass(): m_renderTargetTexture(nullptr), m_renderTargetView(nullptr), m_shaderResourceView(nullptr), m_depthStencilBuffer(nullptr), m_depthStencilView(nullptr)
{
}


RenderToTextureClass::~RenderToTextureClass()
{
}

bool RenderToTextureClass::Initialize(ID3D11Device* const dev, const int textureWidth, const int textureHeight, const float screendepth, const float screennear)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;

	


	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target texture.
	result = dev->CreateTexture2D(&textureDesc, nullptr, &m_renderTargetTexture);
	if (FAILED(result))
	{
		return false;
	}
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = dev->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = dev->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	if (FAILED(result))
	{
		return false;
	}
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = dev->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = dev->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the viewport for rendering.
	m_viewport.Width = static_cast<float>(textureWidth);
	m_viewport.Height = static_cast<float>(textureHeight);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	// Setup the projection matrix.
	
	m_projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, (static_cast<float>(textureWidth) / static_cast<float>(textureHeight)), screennear, screendepth);
	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH(textureWidth, textureHeight, screennear, screendepth);
	return true;
}

void RenderToTextureClass::SetRenderTarget(ID3D11DeviceContext* const devcon, ID3D11DepthStencilView* const depthStencilView) const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	devcon->OMSetRenderTargets(1, &m_renderTargetView, depthStencilView);
	devcon->RSSetViewports(1, &m_viewport);
}

void RenderToTextureClass::ClearRenderTarget(ID3D11DeviceContext* const devcon, ID3D11DepthStencilView* const depthStencilView, const float red, const float green ,const float blue, const float alpha) const
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	devcon->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* RenderToTextureClass::GetShaderResourceView() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_shaderResourceView;
}

void RenderToTextureClass::GetProjectionMatrix(Matrix& projectionMatrix) const // parasoft-suppress  OPT-24 "Not Included In Spec"
// parasoft-suppress  OPT-24 "Not Included In Spec"
{
	projectionMatrix = m_projectionMatrix;
}

void RenderToTextureClass::GetOrthoMatrix(Matrix& orthoMatrix) const // parasoft-suppress  OPT-24 "Not Included In Spec"
// parasoft-suppress  OPT-24 "Not Included In Spec"
{
	orthoMatrix = m_orthoMatrix;
}
