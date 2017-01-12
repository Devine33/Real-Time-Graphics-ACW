#include "CubeClass.h"
#include "TextureClass.h"


CubeClass::CubeClass(): m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_vertexCount(0), m_indexCount(0), vertices(nullptr), indices(nullptr), result(0), stride(0), offset(0), m_Texture(nullptr), m_model(nullptr)
{
}

CubeClass::~CubeClass()
{
	delete[] vertices;
	delete[] indices;
	delete m_Texture;
	delete[] m_model;
}

bool CubeClass::Initialize(ID3D11Device*const dev, const char*const modelFilename, const WCHAR*const textureFilename)
{

	// Load in the model data,
	LoadModel(modelFilename);

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	InitializeBuffers(dev);


	// Load the texture for this model.
	LoadTexture(dev, textureFilename);


	return true;
}


void CubeClass::Render(ID3D11DeviceContext*const devcon) // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	RenderBuffers(devcon);
}

int CubeClass::GetIndexCount() const // parasoft-suppress  OPT-23 "Not Included In Spec"
{
	return m_indexCount;
}

ID3D11ShaderResourceView* CubeClass::GetTexture() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	return m_Texture->GetTexture();
}

bool CubeClass::InitializeBuffers(ID3D11Device*const dev)
{
	int i;
	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = Vector3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = Vector2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = Vector3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}


	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
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

void CubeClass::RenderBuffers(ID3D11DeviceContext*const devcon)
{
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	devcon->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	devcon->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool CubeClass::LoadModel(const char*const filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

bool CubeClass::LoadTexture(ID3D11Device*const dev, const WCHAR*const filename)
{

	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	m_Texture->Initialize(dev, filename);


	return true;
}
