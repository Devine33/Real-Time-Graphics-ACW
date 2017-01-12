#pragma once
//referenced from rastertek
//allowed me to understand how render to texture works
#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;
class DebugWindowClass
{
public:
	DebugWindowClass();
	DebugWindowClass(const DebugWindowClass&) = default;
	DebugWindowClass& operator=(const DebugWindowClass&) = default;
	~DebugWindowClass();

	bool Initialize(ID3D11Device* const dev ,const int,const int,const int,const int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*const devcon, const int, const int);

	int GetIndexCount() const;

private:
	bool InitializeBuffers(ID3D11Device*const );
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*const, const int,const int);
	void RenderBuffers(ID3D11DeviceContext*const deviceContext) const;

	struct VertexType
	{
		Vector3 position;
		Vector2 texture;

	};

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;

};

