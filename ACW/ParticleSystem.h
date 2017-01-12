#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
#include "TextureClass.h"
using namespace DirectX;
using namespace SimpleMath;
class ParticleSystem /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{
public:
	ParticleSystem();
	ParticleSystem(const ParticleSystem&) = default;
	ParticleSystem& operator=(const ParticleSystem&) = default;
	~ParticleSystem();
	bool Initialize(ID3D11Device* const , const WCHAR* const);
	void Shutdown();
	bool Frame(const float, ID3D11DeviceContext* const);
	void Render(ID3D11DeviceContext* const) const;

	ID3D11ShaderResourceView* GetTexture() const;
	int GetIndexCount() const;


private:
	bool LoadTexture(ID3D11Device* const,const  WCHAR* const);
	void ReleaseTexture();

	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffers(ID3D11Device* const deviceContext);
	void ShutdownBuffers();

	void EmitParticles(const float);
	void UpdateParticles(const float) const;
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext* const);

	void RenderBuffers(ID3D11DeviceContext* const) const;

	struct ParticleType
	{
		float positionX, positionY, positionZ;
		float red, green, blue;
		float velocity;
		bool active;
	};
	struct VertexType
	{
		Vector3 position;
		Vector2 texture;
		Vector4 color;
	};

	float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	float m_particleVelocity, m_particleVelocityVariation;
	float m_particleSize, m_particlesPerSecond;
	int m_maxParticles;

	int m_currentParticleCount;
	float m_accumulatedTime;
	TextureClass* m_Texture;
	ParticleType* m_particleList;

	int m_vertexCount, m_indexCount;
	VertexType* m_vertices;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
};

