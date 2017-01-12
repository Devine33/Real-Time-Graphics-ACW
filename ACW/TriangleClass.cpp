#include "TriangleClass.h"



TriangleClass::TriangleClass(): m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_vertexCount(0), m_indexCount(0), vertices(nullptr), lighting(nullptr), indices(nullptr), result(0), stride(0), offset(0), m_texture(nullptr)
{
}



TriangleClass::~TriangleClass()
{
	delete[] lighting;
	delete[] indices;
	delete m_texture;
}

bool TriangleClass::Initialize(ID3D11Device*const dev) // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	InitializeBuffers(dev);
	return true;
}
//call this for light 
bool TriangleClass::InitializeTexture(ID3D11Device*const dev, const WCHAR*const textureFilename)
{
	
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	InitializeBuffers(dev);

	
	// Load the texture for this model.
	LoadTexture(dev, textureFilename);


	return true;
}


void TriangleClass::Render(ID3D11DeviceContext*const devcon) // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	RenderBuffers(devcon);
}

int TriangleClass::GetIndexCount() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_indexCount;
}

ID3D11ShaderResourceView* TriangleClass::GetTexture() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	
	return m_texture->GetTexture();
}

bool TriangleClass::InitializeBuffers(ID3D11Device*const dev)
{
	// Set the number of vertices in the vertex array.
	m_vertexCount = 3;

	// Set the number of indices in the index array.
	m_indexCount = 3;


	lighting = new LightType[m_vertexCount];
	if (!lighting)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	//// Load the vertex array with data.
	//vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	//vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//textures[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//textures[0].texture = XMFLOAT2(0.0f, 1.0);

	//textures[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	//textures[1].texture = XMFLOAT2(0.5,0.0f);

	//textures[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//textures[2].texture = XMFLOAT2(1.0f,1.0f);

	lighting[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	lighting[0].texture = XMFLOAT2(0.0f, 1.0);
	lighting[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	lighting[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	lighting[1].texture = XMFLOAT2(0.5, 0.0f);
	lighting[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	lighting[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	lighting[2].texture = XMFLOAT2(1.0f, 1.0f);
	lighting[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(LightType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = lighting;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	dev->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	dev->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	return true;
}

void TriangleClass::RenderBuffers(ID3D11DeviceContext*const devcon)
{
	//Ensure stride size matches bytewidth
	stride = sizeof(LightType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	devcon->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	devcon->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool TriangleClass::LoadTexture(ID3D11Device*const dev, const WCHAR*const file)
{

	// Create the texture object.
	m_texture = new TextureClass;
	if (!m_texture)
	{
		return false;
	}

	// Initialize the texture object.
	m_texture->Initialize(dev, file);
	return true;
}
