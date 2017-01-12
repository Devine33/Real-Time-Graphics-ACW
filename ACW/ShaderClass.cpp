#include "ShaderClass.h"
#include <fstream>

ShaderClass::ShaderClass(): m_vertexShader(nullptr), m_pixelShader(nullptr), m_layout(nullptr), m_matrixBuffer(nullptr), m_blob(nullptr), errorMessage(nullptr), vertexShaderBuffer(nullptr), pixelShaderBuffer(nullptr), Layout{nullptr}, numElements(0),dataPtr(nullptr)
{
}

ShaderClass::ShaderClass(const ShaderClass&): m_vertexShader(nullptr), m_pixelShader(nullptr), m_layout(nullptr), m_matrixBuffer(nullptr), m_blob(nullptr), errorMessage(nullptr), vertexShaderBuffer(nullptr), pixelShaderBuffer(nullptr), Layout{nullptr}, numElements(0),dataPtr(nullptr)
{
}

ShaderClass::~ShaderClass()
{
}

bool ShaderClass::Initialize(ID3D11Device*const dev)
{
	InitializeVertexShader(dev, L"../ACW/Shaders/color.vs", "ColorVertexShader");
	InitializePixelShader(dev, L"../ACW/Shaders/color.ps", "ColorPixelShader");
	return true;
}

bool ShaderClass::Render(ID3D11DeviceContext*const devcon,const int indexCount,const XMMATRIX& world,const XMMATRIX& view ,const XMMATRIX& projection)
{
	SetShaderParameters(devcon,world,view,projection);
	RenderShader(devcon, indexCount);
	return true;
}

bool ShaderClass::InitializeVertexShader(ID3D11Device*const dev, const wchar_t*const filename,const  LPCSTR entrypoint)
{
	HRESULT result;
	result = D3DCompileFromFile(filename, nullptr, nullptr, entrypoint, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);

	//if (FAILED(result))
	//{
	//	// If the shader failed to compile it should have writen something to the error message.
	//	if (errorMessage)
	//	{
	//		OutputShaderErrorMessage(errorMessage, hwnd, filename);
	//	}
	//	// If there was  nothing in the error message then it simply could not find the shader file itself.
	//	else
	//	{
	//		MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
	//	}

	//	return false;
	//}

	dev->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	return true;
}

bool ShaderClass::InitializePixelShader(ID3D11Device*const dev, const wchar_t*const filename,const LPCSTR entrypoint)
{
	D3DCompileFromFile(filename, nullptr, nullptr, entrypoint, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	dev->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);
	return true;
}

//void ShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
//{
//	char* compileErrors;
//	unsigned long long bufferSize, i;
//	std::ofstream fout;
//
//
//	// Get a pointer to the error message text buffer.
//	compileErrors = (char*)(errorMessage->GetBufferPointer());
//
//	// Get the length of the message.
//	bufferSize = errorMessage->GetBufferSize();
//
//	// Open a file to write the error message to.
//	fout.open("shader-error.txt");
//
//	// Write out the error message.
//	for (i = 0; i<bufferSize; i++)
//	{
//		fout << compileErrors[i];
//	}
//
//	// Close the file.
//	fout.close();
//
//	// Release the error message.
//	errorMessage->Release();
//
//	// Pop a message up on the screen to notify the user to check the text file for compile errors.
//	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
//
//}

bool ShaderClass::SetShaderParameters(ID3D11DeviceContext*const devcon,const XMMATRIX& worldin,const XMMATRIX& viewin,const XMMATRIX& projectionin)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	XMMATRIX world = worldin;
	XMMATRIX view = viewin;
	XMMATRIX projection = projectionin;


	world= XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);
	devcon->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	
	dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	// Unlock the constant buffer.
	devcon->Unmap(m_matrixBuffer, 0);
	devcon->VSSetConstantBuffers(0, 1, &m_matrixBuffer);
	return true;
}

void ShaderClass::RenderShader(ID3D11DeviceContext*const devcon,const int indexcount) const
{
	devcon->IASetInputLayout(m_layout);

	//set shaders
	devcon->VSSetShader(m_vertexShader, nullptr, 0);
	devcon->PSSetShader(m_pixelShader, nullptr, 0);

	devcon->DrawIndexed(indexcount, 0, 0);
}


