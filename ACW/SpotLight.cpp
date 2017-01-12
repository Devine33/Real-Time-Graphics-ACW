#include "SpotLight.h"



SpotLight::SpotLight(): m_vertexShader(nullptr), m_pixelShader(nullptr), m_layout(nullptr), m_sampleState(nullptr), m_matrixBuffer(nullptr), m_lightColorBuffer(nullptr), m_lightPositionBuffer(nullptr), pixelShaderBuffer(nullptr), errorMessage(nullptr)
{
}


SpotLight::~SpotLight()
{
}

bool SpotLight::Initialize(ID3D11Device*const device)
{
	bool result;
	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, L"../ACW/Shaders/SpotLightShaders/light.vs", L"../ACW//Shaders/SpotLightShaders/light.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

bool SpotLight::Render(ID3D11DeviceContext*const deviceContext, const int indexCount, const Matrix& worldMatrix, const Matrix& viewMatrix, const Matrix& projectionMatrix, ID3D11ShaderResourceView*const texture, const Vector4*const diffuseColor, const Vector4*const lightPosition) const
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

bool SpotLight::InitializeShader(ID3D11Device*const dev, const WCHAR*const, const WCHAR*const)
{
	InitializeVertexShader(dev,L"../ACW/Shaders/SpotLightShaders/light.vs", "LightVertexShader");
	InitializePixelShader(dev, L"../ACW//Shaders/SpotLightShaders/light.ps", "LightPixelShader");
	return true;
}

bool SpotLight::InitializeVertexShader(ID3D11Device*const dev, const wchar_t*const filename, const LPCSTR entrypoint)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	
	



	result = D3DCompileFromFile(filename, nullptr, nullptr, entrypoint, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		return false;
	}
	// Create the vertex shader from the buffer.
	result = dev->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr,
		&m_vertexShader);

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
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
	result = dev->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	D3D11_SAMPLER_DESC samplerDesc;
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

	// Create the texture sampler state.
	result = dev->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC matrixBufferDesc;
	// Setup the description of the matrix dynamic constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the matrix constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = dev->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC lightColorBufferDesc;
	// Setup the description of the dynamic constant buffer that is in the pixel shader.
	lightColorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightColorBufferDesc.ByteWidth = sizeof(LightColorBufferType);
	lightColorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightColorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightColorBufferDesc.MiscFlags = 0;
	lightColorBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = dev->CreateBuffer(&lightColorBufferDesc, nullptr, &m_lightColorBuffer);
	if (FAILED(result))
	{
		return false;
	}
	D3D11_BUFFER_DESC lightPositionBufferDesc;

	// Setup the description of the dynamic constant buffer that is in the vertex shader.
	lightPositionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightPositionBufferDesc.ByteWidth = sizeof(LightPositionBufferType);
	lightPositionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightPositionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightPositionBufferDesc.MiscFlags = 0;
	lightPositionBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = dev->CreateBuffer(&lightPositionBufferDesc, nullptr, &m_lightPositionBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool SpotLight::InitializePixelShader(ID3D11Device*const dev, const wchar_t*const filename, const LPCSTR entrypoint)
{
	HRESULT result;
	result = D3DCompileFromFile(filename, nullptr, nullptr, entrypoint, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		return false;
	}
	dev->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);
	return true;
}

bool SpotLight::SetShaderParameters(ID3D11DeviceContext*const devcon, const Matrix& worldin, const Matrix& viewin, const Matrix& projectionin, ID3D11ShaderResourceView*const texture, const Vector4*const diffuse, const Vector4*const lightPosition) const
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightPositionBufferType* dataPtr2;
	LightColorBufferType* dataPtr3;
	unsigned int bufferNumber;
	Matrix world = worldin;
	Matrix view = viewin;
	Matrix projection = projectionin;

	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);
	devcon->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	// Unlock the matrix constant buffer.
	devcon->Unmap(m_matrixBuffer, 0);

	// Set the position of the matrix constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the matrix constant buffer in the vertex shader with the updated values.
	devcon->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Lock the light position constant buffer so it can be written to.
	result = devcon->Map(m_lightPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	// Get a pointer to the data in the constant buffer.
	dataPtr2 = static_cast<LightPositionBufferType*>(mappedResource.pData);

	// Copy the light position variables into the constant buffer.
	dataPtr2->lightPosition[0] = lightPosition[0];
	dataPtr2->lightPosition[1] = lightPosition[1];
	dataPtr2->lightPosition[2] = lightPosition[2];
	dataPtr2->lightPosition[3] = lightPosition[3];

	// Unlock the constant buffer.
	devcon->Unmap(m_lightPositionBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 1;

	// Finally set the constant buffer in the vertex shader with the updated values.
	devcon->VSSetConstantBuffers(bufferNumber, 1, &m_lightPositionBuffer);

	// Set shader texture resource in the pixel shader.
	devcon->PSSetShaderResources(0, 1, &texture);

	// Lock the light color constant buffer so it can be written to.
	result = devcon->Map(m_lightColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr3 = static_cast<LightColorBufferType*>(mappedResource.pData);

	// Copy the light color variables into the constant buffer.
	dataPtr3->diffuseColor[0] = diffuse[0];
	dataPtr3->diffuseColor[1] = diffuse[1];
	dataPtr3->diffuseColor[2] = diffuse[2];
	dataPtr3->diffuseColor[3] = diffuse[3];

	// Unlock the constant buffer.
	devcon->Unmap(m_lightColorBuffer, 0);

	// Set the position of the constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the constant buffer in the pixel shader with the updated values.
	devcon->PSSetConstantBuffers(bufferNumber, 1, &m_lightColorBuffer);

	return true;
}

void SpotLight::RenderShader(ID3D11DeviceContext*const devcon, const int indexCount) const
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
