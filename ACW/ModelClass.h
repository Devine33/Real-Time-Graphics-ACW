#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <fstream>
#include "TextureClass.h"
using namespace std;
using namespace DirectX;
using namespace SimpleMath;
class ModelClass /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{
public:
	ModelClass();
	ModelClass(const ModelClass&) = default;
	ModelClass& operator=(const ModelClass&) = default;
	~ModelClass();

	bool Initialize(ID3D11Device*const dev);
	bool InitializeTexture(ID3D11Device*const dev, const char*const modelFilename, const WCHAR*const textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext*const devcon) const;

	int GetIndexCount() const;
	ID3D11ShaderResourceView* GetTexture() const;

private:
	bool InitializeBuffers(ID3D11Device*const dev);
	void RenderBuffers(ID3D11DeviceContext*const devcon) const;
	bool LoadTexture(ID3D11Device*const dev, const WCHAR*const file);
	bool LoadModel(const char*const);
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
	TextureClass* m_Texture;
	ModelType* m_model;
};

