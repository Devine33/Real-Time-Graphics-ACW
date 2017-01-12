#pragma once
//referenced from rastertek
//used to generate a cube model for testing shadows 16-11-2016
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include "TextureClass.h"
#include <SimpleMath.h>
using namespace std;
using namespace DirectX;
using namespace SimpleMath;
class CubeClass /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{
public:
	CubeClass();
	CubeClass(const CubeClass&) = default;
	CubeClass& operator=(const CubeClass&) = default;

	~CubeClass();
	bool Initialize(ID3D11Device*const dev, const char*const modelFilename, const WCHAR*const textureFilename);
	void Cleanup();
	void Render(ID3D11DeviceContext*const devcon);
	int  GetIndexCount() const;
	ID3D11ShaderResourceView* GetTexture() const;
private:
	bool InitializeBuffers(ID3D11Device*const dev);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*const devcon);
	bool LoadModel(const char*const);
	bool LoadTexture(ID3D11Device*const, const WCHAR*const);
	void ReleaseModel();


	struct VertexType
	{
		Vector3 position;
		Vector2 texture;
		Vector3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	unsigned int stride, offset;
	TextureClass *m_Texture;
	ModelType* m_model;
};

