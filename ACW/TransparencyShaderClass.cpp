#include "TransparencyShaderClass.h"



TransparencyShaderClass::TransparencyShaderClass(): m_vertexShader(nullptr), m_pixelShader(nullptr), m_layout(nullptr), m_blob(nullptr), m_sampleState(nullptr), errorMessage(nullptr), vertexShaderBuffer(nullptr), pixelShaderBuffer(nullptr), numElements(0), dataPtr(nullptr), dataPtr2(nullptr), m_matrixBuffer(nullptr),m_transparentBuffer(nullptr)
{
}


TransparencyShaderClass::~TransparencyShaderClass()
{
}

bool TransparencyShaderClass::Initialize(ID3D11Device*const dev, const HWND hwnd)
{
	HRESULT result;
	// Initialize the vertex and pixel shaders.
	result = InitializeShader(dev, hwnd, L"../ACW/Shaders/TransparencyShader/transparency.vs", L"../ACW/Shaders/TransparencyShader/transparency.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

bool TransparencyShaderClass::Render(ID3D11DeviceContext*const devcon, const int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView*const texture, const float blend)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(devcon, world, view, projection, texture, blend);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(devcon, indexCount);

	return true;
}

bool TransparencyShaderClass::InitializeVertexShader(ID3D11Device*const dev, const HWND hwnd, const wchar_t*const filename, const LPCSTR entrypoint)
{
	HRESULT result;
	result =  D3DCompileFromFile(filename, nullptr, nullptr, entrypoint, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			//OutputShaderErrorMessage(errorMessage, hwnd, filename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	//must match sizes of vectors 
	dev->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	numElements = ARRAYSIZE(layout);
	dev->CreateInputLayout(layout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	dev->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	dev->CreateSamplerState(&samplerDesc, &m_sampleState);

	// Setup the description of the transparent dynamic constant buffer that is in the pixel shader.
	transparentBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	transparentBufferDesc.ByteWidth = sizeof(TransparentBufferType);
	transparentBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transparentBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	transparentBufferDesc.MiscFlags = 0;
	transparentBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = dev->CreateBuffer(&transparentBufferDesc, nullptr, &m_transparentBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool TransparencyShaderClass::InitializePixelShader(ID3D11Device*const dev, const wchar_t*const filename, const LPCSTR entrypoint)
{
	D3DCompileFromFile(filename, nullptr, nullptr, entrypoint, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	dev->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);
	return true;
}

bool TransparencyShaderClass::InitializeShader(ID3D11Device*const dev, const HWND hwnd, const WCHAR*const, const WCHAR*const)
{
	InitializeVertexShader(dev, hwnd, L"../ACW/Shaders/TransparencyShader/Transparency.vs", "TransparentVertexShader");
	InitializePixelShader(dev, L"../ACW/Shaders/TransparencyShader/Transparency.ps", "TransparentPixelShader");
	return true;
}

bool TransparencyShaderClass::SetShaderParameters(ID3D11DeviceContext*const devcon, const XMMATRIX& worldin, const XMMATRIX& viewin, const XMMATRIX& projectionin, ID3D11ShaderResourceView*const texture, const float blend)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;

	XMMATRIX world = worldin;
	XMMATRIX view = viewin;
	XMMATRIX projection = projectionin;


	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	// Lock the matrix constant buffer so it can be written to.
	 devcon->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


	// Get a pointer to the data in the matrix constant buffer.
	dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);

	// Copy the matrices into the matrix constant buffer.
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	// Unlock the buffer.
	devcon->Unmap(m_matrixBuffer, 0);

	// Set the position of the matrix constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the matrix constant buffer in the vertex shader with the updated values.
	devcon->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Set shader texture resource in the pixel shader.
	devcon->PSSetShaderResources(0, 1, &texture);

	// Lock the transparent constant buffer so it can be written to.
	 devcon->Map(m_transparentBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the transparent constant buffer.
	dataPtr2 = static_cast<TransparentBufferType*>(mappedResource.pData);

	// Copy the blend amount value into the transparent constant buffer.
	dataPtr2->blendAmount = blend;

	// Unlock the buffer.
	devcon->Unmap(m_transparentBuffer, 0);

	// Set the position of the transparent constant buffer in the pixel shader.
	bufferNumber = 0;

	// Now set the texture translation constant buffer in the pixel shader with the updated values.
	devcon->PSSetConstantBuffers(bufferNumber, 1, &m_transparentBuffer);

	return true;
}

void TransparencyShaderClass::RenderShader(ID3D11DeviceContext*const devcon, const int indexCount) const
{
	// Set the vertex input layout.
	devcon->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(m_vertexShader, nullptr, 0);
	devcon->PSSetShader(m_pixelShader, nullptr, 0);

	// Set the sampler state in the pixel shader.
	devcon->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	devcon->DrawIndexed(indexCount, 0, 0);
}
