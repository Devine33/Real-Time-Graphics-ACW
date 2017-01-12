#include "ShadowShaderClass.h"



ShadowShaderClass::ShadowShaderClass(): m_vertexShader(nullptr), m_pixelShader(nullptr), m_layout(nullptr), m_sampleStateWrap(nullptr), pixelShaderBuffer(nullptr), errorMessage(nullptr), m_sampleStateClamp(nullptr), m_matrixBuffer(nullptr), m_lightBuffer(nullptr), m_lightBuffer2(nullptr)
{
}


ShadowShaderClass::~ShadowShaderClass()
{
}

bool ShadowShaderClass::Initialize(ID3D11Device*const dev) // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	// Initialize the vertex and pixel shaders.
	InitializeShader(dev, L"../ACW/Shaders/LightShaders/light.vs", L"../ACW/Shaders/LightShaders/light.ps");
	return true;
}

bool ShadowShaderClass::Render(ID3D11DeviceContext*const devcon, const int indexCount, const Matrix& world, const Matrix& view, const Matrix& projection, const Matrix& lightViewMatrix, const Matrix& lightProjectionMatrix, ID3D11ShaderResourceView*const texture, ID3D11ShaderResourceView*const depthMapTexture, const Vector3& lightPosition, const Vector4& ambientColor, const Vector4& diffuseColor) const
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(devcon, world, view, projection, lightViewMatrix, lightProjectionMatrix, texture,
	                             depthMapTexture, lightPosition, ambientColor, diffuseColor);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(devcon, indexCount);

	return true;
}

bool ShadowShaderClass::InitializeVertexShader(ID3D11Device*const dev, const wchar_t*const filename, const LPCSTR entrypoint)
{
	HRESULT result;

	ID3D10Blob* vertexShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	




	result = D3DCompileFromFile(filename, nullptr, nullptr, entrypoint, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	//must match sizes of vectors 
	dev->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);


	// Create the vertex input layout description.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = dev->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);


	// Create a wrap texture sampler state description.
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

	// Create the texture sampler state.
	result = dev->CreateSamplerState(&samplerDesc, &m_sampleStateWrap);
	if (FAILED(result))
	{
		return false;
	}

	// Create a clamp texture sampler state description.
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	// Create the texture sampler state.
	result = dev->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);
	if (FAILED(result))
	{
		return false;
	}
	D3D11_BUFFER_DESC matrixBufferDesc;
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = dev->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}
	D3D11_BUFFER_DESC lightBufferDesc;
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = dev->CreateBuffer(&lightBufferDesc, nullptr, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}
	D3D11_BUFFER_DESC lightBufferDesc2;
	// Setup the description of the light dynamic constant buffer that is in the vertex shader.
	lightBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc2.ByteWidth = sizeof(LightBufferType2);
	lightBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc2.MiscFlags = 0;
	lightBufferDesc2.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = dev->CreateBuffer(&lightBufferDesc2, nullptr, &m_lightBuffer2);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool ShadowShaderClass::InitializePixelShader(ID3D11Device*const dev, const wchar_t*const filename, const LPCSTR entrypoint)
{
	D3DCompileFromFile(filename, nullptr, nullptr, entrypoint, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	dev->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);
	return true;
}

bool ShadowShaderClass::InitializeShader(ID3D11Device*const dev, const WCHAR*const, const WCHAR*const)
{
	InitializeVertexShader(dev, L"../ACW/Shaders/ShadowShaders/shadow.vs", "ShadowVertexShader");
	InitializePixelShader(dev, L"../ACW/Shaders/ShadowShaders/shadow.ps", "ShadowPixelShader");
	return true;
}

bool ShadowShaderClass::SetShaderParameters(ID3D11DeviceContext*const devcon, const Matrix& worldin, const Matrix& viewin, const Matrix& projectionin, const Matrix& lightviewin, const Matrix& lightprojectionin, ID3D11ShaderResourceView*const texture, ID3D11ShaderResourceView*const depthMapTexture, const Vector3& lightPosition, const Vector4& ambientColor, const Vector4& diffuseColor) const
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	LightBufferType2* dataPtr3;

	Matrix world = worldin;
	Matrix view = viewin;
	Matrix projection = projectionin;

	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	Matrix lightview = lightviewin;
	Matrix lightprojection = lightprojectionin;
	lightview = XMMatrixTranspose(lightview);
	lightprojection = XMMatrixTranspose(lightprojection);

	result = devcon->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;
	dataPtr->lightView = lightview;
	dataPtr->lightProjection = lightprojection;

	// Unlock the constant buffer.
	devcon->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	devcon->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Set shader texture resource in the pixel shader.
	devcon->PSSetShaderResources(0, 1, &texture);

	devcon->PSSetShaderResources(1, 1, &depthMapTexture);

	// Lock the light constant buffer so it can be written to.
	result = devcon->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = static_cast<LightBufferType*>(mappedResource.pData);

	// Copy the lighting variables into the constant buffer.
	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;

	// Unlock the constant buffer.
	devcon->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	devcon->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	// Lock the second light constant buffer so it can be written to.
	result = devcon->Map(m_lightBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr3 = static_cast<LightBufferType2*>(mappedResource.pData);

	// Copy the lighting variables into the constant buffer.
	dataPtr3->lightPosition = lightPosition;
	dataPtr3->padding = 0.0f;

	// Unlock the constant buffer.
	devcon->Unmap(m_lightBuffer2, 0);

	// Set the position of the light constant buffer in the vertex shader.
	bufferNumber = 1;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	devcon->VSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer2);

	return true;
}

void ShadowShaderClass::RenderShader(ID3D11DeviceContext*const devcon, const int indexCount) const
{
	// Set the vertex input layout.
	devcon->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(m_vertexShader, nullptr, 0);
	devcon->PSSetShader(m_pixelShader, nullptr, 0);

	// Set the sampler states in the pixel shader.
	devcon->PSSetSamplers(0, 1, &m_sampleStateClamp);
	devcon->PSSetSamplers(1, 1, &m_sampleStateWrap);

	// Render the triangle.
	devcon->DrawIndexed(indexCount, 0, 0);
}
