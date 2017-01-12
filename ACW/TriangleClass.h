#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include "TextureClass.h"
using namespace DirectX;
class TriangleClass /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{
public:
	TriangleClass();
	TriangleClass(const TriangleClass&) = default;
	TriangleClass&operator=(const TriangleClass&) = default;
	~TriangleClass();
	bool Initialize(ID3D11Device*const dev);
	bool InitializeTexture(ID3D11Device*const dev, const WCHAR*const textureFilename);
	void Cleanup();
	void Render(ID3D11DeviceContext*const devcon);
	int GetIndexCount() const;
	ID3D11ShaderResourceView* GetTexture() const;
private:
	bool InitializeBuffers(ID3D11Device*const dev);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*const devcon);
	bool LoadTexture(ID3D11Device*const dev, const WCHAR*const file);
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT3 normal;
	};

	struct LightType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	VertexType* vertices;
	LightType* lighting;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	unsigned int stride, offset;
	TextureClass* m_texture;
};

