#include "TextureShaderClass.h"



TextureShaderClass::TextureShaderClass(): m_vertexShader(nullptr), m_pixelShader(nullptr), m_layout(nullptr),
                                          m_blob(nullptr), m_sampleState(nullptr), errorMessage(nullptr), vertexShaderBuffer(nullptr), pixelShaderBuffer(nullptr),
                                          numElements(0), dataPtr(nullptr), dataPtr2(nullptr), dataPtr3(nullptr), m_matrixBuffer(nullptr), m_cameraBuffer(nullptr),m_lightBuffer(nullptr)
{
}


TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialize(ID3D11Device*const dev, const HWND hwnd) // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	// Initialize the vertex and pixel shaders.
	 InitializeShader(dev, hwnd, L"../ACW/Shaders/LightShaders/light.vs", L"../ACW/Shaders/LightShaders/light.ps");
	return true;
}



bool TextureShaderClass::Render(ID3D11DeviceContext*const devcon, const int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView*const texture, const Vector3& direction, const Vector4& colour, const Vector4& ambient, const Vector3& cameraposition, const Vector4& specularColor, const float specularPower)
{
	// Set the shader parameters that it will use for rendering.
	SetShaderParameters(devcon, world, view, projection, texture,direction,colour, ambient,cameraposition,specularColor,specularPower);
	// Now render the prepared buffers with the shader.
	RenderShader(devcon, indexCount);
	return true;
}

bool TextureShaderClass::InitializeVertexShader(ID3D11Device*const dev, const HWND hwnd, const wchar_t*const filename, const LPCSTR entrypoint)
{
	HRESULT result;
	
	result = D3DCompileFromFile(filename, nullptr, nullptr, entrypoint, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
	D3D11_BUFFER_DESC cameraBufferDesc;
	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = dev->CreateBuffer(&cameraBufferDesc, nullptr, &m_cameraBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = dev->CreateBuffer(&lightBufferDesc, nullptr, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}
	// Create the texture sampler state.

	return true;
}

bool TextureShaderClass::InitializePixelShader(ID3D11Device*const dev, const wchar_t*const filename, const LPCSTR entrypoint)
{
	D3DCompileFromFile(filename, nullptr, nullptr, entrypoint, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	dev->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);
	return true;
}

bool TextureShaderClass::InitializeShader(ID3D11Device*const dev, const HWND hwnd, const WCHAR*const, const WCHAR*const)
{
	InitializeVertexShader(dev, hwnd, L"../ACW/Shaders/LightShaders/light.vs", "LightVertexShader");
	InitializePixelShader(dev, L"../ACW/Shaders/LightShaders/light.ps", "LightPixelShader");
	return true;
}

bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext*const devcon, const XMMATRIX& worldin, const XMMATRIX& viewin, const XMMATRIX& projectionin, ID3D11ShaderResourceView*const texture, const Vector3& direction, const Vector4& colour, const Vector4& ambient, const Vector3& cameraposition, const Vector4& specularColor, const float power)
{
	unsigned int bufferNumber;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Matrix world = worldin;
	Matrix view = viewin;
	Matrix projection = projectionin;
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	devcon->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);
	// Copy the matrices into the constant buffer.
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	// Unlock the constant buffer.
	devcon->Unmap(m_matrixBuffer, 0);
	bufferNumber = 0;
	devcon->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);


	// Lock the camera constant buffer so it can be written to.
	devcon->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


	// Get a pointer to the data in the constant buffer.
	dataPtr3 = static_cast<CameraBufferType*>(mappedResource.pData);

	// Copy the camera position into the constant buffer.
	dataPtr3->cameraPosition = cameraposition;
	dataPtr3->padding = 0.0f;
	devcon->Unmap(m_cameraBuffer, 0);
	// Set the position of the camera constant buffer in the vertex shader.
	bufferNumber = 1;
	devcon->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

	// Set shader texture resource in the pixel shader.
	devcon->PSSetShaderResources(0, 1, &texture);

	// Lock the light constant buffer so it can be written to.
	devcon->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = static_cast<LightBufferType*>(mappedResource.pData);

	// Copy the lighting variables into the constant buffer.
	dataPtr2->ambientColor = ambient;
	dataPtr2->diffuseColor = colour;
	dataPtr2->lightDirection = direction;
	dataPtr2->specularColor = specularColor;
	dataPtr2->specularPower = power;

	// Unlock the constant buffer.
	devcon->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	devcon->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);
	return true;
}

void TextureShaderClass::RenderShader(ID3D11DeviceContext*const devcon, const int indexCount) const
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
