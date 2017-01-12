#include "DirectXClass.h"
#include <minwinbase.h>


DirectXClass::DirectXClass(): m_dev(nullptr), m_devcon(nullptr), m_swapchain(nullptr), m_factory(nullptr), m_adapter(nullptr), m_adapterOutput(nullptr), m_numModes(0), i(0), m_numerator(0), m_denominator(0), m_displayModeList(nullptr), m_backBufferPointer(nullptr), m_renderTargetView(nullptr), m_depthStencilBuffer(nullptr), m_depthStencilState(nullptr), m_depthStencilView(nullptr), m_rasterizerState(nullptr), m_wireframeRasterizer(nullptr), m_featureLevel(), m_ParticleEnable(nullptr), m_alphaEnableBlendingState(nullptr), m_alphaDisableBlendingState(nullptr), m_depthDisabledStencilState(nullptr), color{0.0f,0.0f,0.0f,0.0f}
{
}


DirectXClass::~DirectXClass()
{
	delete[] m_displayModeList;
}

bool DirectXClass::DirectXInitialize(const HWND hwnd, const int ScreenWidth, const int ScreenHeight, const bool Fullscreen)
{
	HRESULT result;


	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&m_factory));
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = m_factory->EnumAdapters(0, &m_adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = m_adapter->EnumOutputs(0, &m_adapterOutput);
	if (FAILED(result))
	{
		return false;
	}


	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = m_adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &m_numModes, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	m_displayModeList = new DXGI_MODE_DESC[m_numModes];
	if (!m_displayModeList)
	{
		return false;
	}
	// Now fill the display mode list structures.
	result = m_adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &m_numModes, m_displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < m_numModes; i++)
	{
		if (m_displayModeList[i].Width == static_cast<unsigned int>(ScreenWidth))
		{
			if (m_displayModeList[i].Height == static_cast<unsigned int>(ScreenHeight))
			{
				m_numerator = m_displayModeList[i].RefreshRate.Numerator;
				m_denominator = m_displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Release the display mode list.
	delete[] m_displayModeList;
	m_displayModeList = nullptr;

	// Release the adapter output.
	m_adapterOutput->Release();
	m_adapterOutput = nullptr;

	// Release the adapter.
	m_adapter->Release();
	m_adapter = nullptr;

	// Release the factory.
	m_factory->Release();
	m_factory = nullptr;

	//swapchain fill
	m_desc = {0};
	ZeroMemory(&m_desc, sizeof(m_desc));
	m_desc.BufferCount = 1;
	m_desc.BufferDesc.Width = ScreenWidth;
	m_desc.BufferDesc.Height = ScreenHeight;
	m_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_desc.BufferDesc.RefreshRate.Numerator = 60;
	m_desc.BufferDesc.RefreshRate.Denominator = 1;
	m_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_desc.OutputWindow = hwnd;
	m_desc.SampleDesc.Count = 1;
	m_desc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (Fullscreen)
	{
		m_desc.Windowed = false;
	}
	else
	{
		m_desc.Windowed = true;
	}
	//m_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//m_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//m_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//m_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	// Set the feature level to DirectX 11.
	m_featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
	                                       &m_featureLevel, 1, D3D11_SDK_VERSION, &m_desc,
	                                       &m_swapchain, &m_dev, nullptr, &m_devcon);
	if (FAILED(result))
	{
		return false;
	}
	// Get the pointer to the back buffer.
	result = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&m_backBufferPointer));
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = m_dev->CreateRenderTargetView(m_backBufferPointer, nullptr, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}
	m_backBufferPointer->Release();

	// Initialize the description of the depth buffer.
	ZeroMemory(&m_depthStencilBufferDescription, sizeof(D3D11_TEXTURE2D_DESC));

	// Set up the description of the depth buffer.
	m_depthStencilBufferDescription.ArraySize = 1;
	m_depthStencilBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_depthStencilBufferDescription.CPUAccessFlags = 0; // No CPU access required.
	m_depthStencilBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_depthStencilBufferDescription.Width = ScreenWidth;
	m_depthStencilBufferDescription.Height = ScreenHeight;
	m_depthStencilBufferDescription.MipLevels = 1;
	m_depthStencilBufferDescription.SampleDesc.Count = 1;
	m_depthStencilBufferDescription.SampleDesc.Quality = 0;
	m_depthStencilBufferDescription.Usage = D3D11_USAGE_DEFAULT;

	// Create the texture for the depth buffer using the filled out description.
	m_dev->CreateTexture2D(&m_depthStencilBufferDescription, nullptr, &m_depthStencilBuffer);

	if (FAILED(result))
		return result;

	ZeroMemory(&m_depthStencilDescription, sizeof(D3D11_DEPTH_STENCIL_DESC));

	m_depthStencilDescription.DepthEnable = TRUE;
	m_depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	m_depthStencilDescription.StencilEnable = TRUE;
	m_depthStencilDescription.StencilReadMask = 0xFF;
	m_depthStencilDescription.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	m_depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	m_depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	m_depthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_depthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	m_depthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_depthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_dev->CreateDepthStencilState(&m_depthStencilDescription, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}
	m_devcon->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&m_DepthStencilViewDescription, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	m_DepthStencilViewDescription.Format = m_depthStencilBufferDescription.Format;
	m_DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	m_DepthStencilViewDescription.Texture2D.MipSlice = 0;
	result = m_dev->CreateDepthStencilView(m_depthStencilBuffer, &m_DepthStencilViewDescription, &m_depthStencilView);

	if (FAILED(result))
		return result;
	m_devcon->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);


	ZeroMemory(&m_rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));

	m_rasterizerDescription.AntialiasedLineEnable = FALSE;
	m_rasterizerDescription.CullMode = D3D11_CULL_NONE;
	m_rasterizerDescription.DepthBias = 0;
	m_rasterizerDescription.DepthBiasClamp = 0.0f;
	m_rasterizerDescription.DepthClipEnable = TRUE;
	m_rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	m_rasterizerDescription.FrontCounterClockwise = FALSE;
	m_rasterizerDescription.MultisampleEnable = FALSE;
	m_rasterizerDescription.ScissorEnable = FALSE;
	m_rasterizerDescription.SlopeScaledDepthBias = 0.0f;

	
	// Create the rasterizer state object.
	//enables wireframe
	m_dev->CreateRasterizerState(&m_rasterizerDescription, &m_rasterizerState);
	m_devcon->RSSetState(m_rasterizerState);

	m_rasterizerDescription.FillMode = D3D11_FILL_WIREFRAME;
	m_dev->CreateRasterizerState(&m_rasterizerDescription, &m_wireframeRasterizer);

	ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));
	m_viewPort.Width = static_cast<FLOAT>(ScreenWidth);
	m_viewPort.Height = static_cast<FLOAT>(ScreenHeight);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_devcon->RSSetViewports(1, &m_viewPort);
	// Setup the projection matrix.
	//fieldOfView = static_cast<float>(XM_PI) / 4.0f;
	//screenAspect = static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight);

	//m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screennear, screenfar);
	//// Create the projection matrix for 3D rendering.
	//m_worldMatrix = XMMatrixIdentity();

	//m_orthoMatrix = XMMatrixOrthographicLH(ScreenWidth, ScreenHeight, screennear, screenfar);



	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = m_dev->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Clear the blend state description.
	ZeroMemory(&m_blendDesc, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	m_blendDesc.RenderTarget[0].BlendEnable = TRUE;
	//transparency shader
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	m_blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	m_blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	m_blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	m_blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	m_blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	m_blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	m_blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	ZeroMemory(&m_particleblendDesc, sizeof(D3D11_BLEND_DESC));

	m_particleblendDesc.RenderTarget[0].BlendEnable = TRUE;
	m_particleblendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	m_particleblendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_particleblendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	m_particleblendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	m_particleblendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	m_particleblendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	m_particleblendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	//m_blendDesc.RenderTarget[1].SrcBlend = D3D11_BLEND_ONE;
	//m_blendDesc.RenderTarget[1].DestBlend = D3D11_BLEND_ONE;
	//m_blendDesc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
	//m_blendDesc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
	//m_blendDesc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;
	//m_blendDesc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//m_blendDesc.RenderTarget[1].RenderTargetWriteMask = 0x0f;

	//create blend states for particles

	m_dev->CreateBlendState(&m_particleblendDesc, &m_ParticleEnable);

	//// Create the blend state using the description.
	result = m_dev->CreateBlendState(&m_blendDesc, &m_alphaEnableBlendingState);
	if(FAILED(result))
	{
		return false;
	}

	//// Modify the description to create an alpha disabled blend state description.
	m_blendDesc.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	result = m_dev->CreateBlendState(&m_blendDesc, &m_alphaDisableBlendingState);
	if(FAILED(result))
	{
		return false;
	}



	return true;
}

void DirectXClass::BeginScene()
{
	const float red = 0.0f;
	const float green = 0.0f;
	const float blue = 1.0f;
	const float alpha = 1.0f;
	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;


	// Clear the back buffer.
	m_devcon->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	m_devcon->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectXClass::EndScene() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	// Present the back buffer to the screen since rendering is complete
	// Lock to screen refresh rate.
	m_swapchain->Present(1, 0);
}

ID3D11Device* DirectXClass::GetDevice() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_dev;
}

ID3D11DeviceContext* DirectXClass::GetDeviceContext() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_devcon;
}

void DirectXClass::TurnOnAlphaBlending() const
{

	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_devcon->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void DirectXClass::TurnOffAlphaBlending() const
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_devcon->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}

void DirectXClass::TurnOnParticleBlend() const
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	// Turn off the alpha blending.
	m_devcon->OMSetBlendState(m_ParticleEnable, blendFactor, 0xffffffff);

}

void DirectXClass::TurnZBufferOn() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	m_devcon->OMSetDepthStencilState(m_depthStencilState, 1);
}



void DirectXClass::TurnZBufferOff() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	m_devcon->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}

void DirectXClass::SetWireFrame() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	m_devcon->RSSetState(m_wireframeRasterizer);
}

void DirectXClass::SetFill() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	m_devcon->RSSetState(m_rasterizerState);
}


void DirectXClass::Cleanup()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapchain)
	{
		m_swapchain->SetFullscreenState(false, nullptr);
	}

	if (m_rasterizerState)
	{
		m_rasterizerState->Release();
		m_rasterizerState = nullptr;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_devcon)
	{
		m_devcon->Release();
		m_devcon = nullptr;
	}

	if (m_dev)
	{
		m_dev->Release();
		m_dev = nullptr;
	}

	if (m_swapchain)
	{
		m_swapchain->Release();
		m_swapchain = nullptr;
	}
}

void DirectXClass::GetProjectionMatrix(SimpleMath::Matrix& projectionMatrix) const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	projectionMatrix = m_projectionMatrix;
	return;
}
//
//void DirectXClass::GetWorldMatrix(SimpleMath::Matrix& worldMatrix) const // parasoft-suppress  OPT-24 "Not Included In Spec"
//{
//	worldMatrix = m_worldMatrix;
//	return;
//}

//void DirectXClass::GetOrthoMatrix(SimpleMath::Matrix& orthoMatrix) const // parasoft-suppress  OPT-24 "Not Included In Spec"
//{
//	orthoMatrix = m_orthoMatrix;
//	return;
//}

void DirectXClass::ResetViewPort() // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	// Set the viewport.
	m_devcon->RSSetViewports(1, &m_viewPort);
}

ID3D11DepthStencilView* DirectXClass::GetDepthStencilView() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_depthStencilView;
}



void DirectXClass::SetBackBufferRenderTarget() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_devcon->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}
