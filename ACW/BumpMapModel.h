#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <fstream>
#include "TextureArrayClass.h"
using namespace std;
using namespace DirectX;
using namespace SimpleMath;
class BumpMapModel
{
public:
	BumpMapModel();
	BumpMapModel(const BumpMapModel&) = default;
	BumpMapModel& operator=(const BumpMapModel&) = default;

	~BumpMapModel();
	 bool Initialize(ID3D11Device*dev,const char*const modelFilename,const WCHAR*const textureFilename1, const WCHAR*const textureFilename2);
	void Shutdown();
	void Render(ID3D11DeviceContext*const devcon) const;

	int GetIndexCount() const;
	ID3D11ShaderResourceView** GetTextureArray() const;
private:
	bool InitializeBuffers(ID3D11Device*const dev);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*const devcon) const;

	bool LoadTextures(ID3D11Device*const dev, const WCHAR*const filename1, const WCHAR*const filename2);
	void ReleaseTextures();

	bool LoadModel(const char* filename);
	void ReleaseModel() const;

	void CalculateModelVectors();



	struct VertexType
	{
		Vector3 position;
		Vector2 texture;
		Vector3 normal;
		Vector3 tangent;
		Vector3 binormal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

	void CalculateTangentBinormal(const TempVertexType&, const TempVertexType&, const TempVertexType&, VectorType&, VectorType&) const;
	void CalculateNormal(const VectorType& tangent, const VectorType& binormal, VectorType& normal) const;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	ModelType* m_model;
	TextureArrayClass* m_TextureArray;
};

