#pragma once
#include <Windows.h>
#include "DirectXClass.h"
#include "CameraClass.h"
#include "TriangleClass.h"
#include "ShaderClass.h"
#include "CubeClass.h"
#include "TextureShaderClass.h"
#include "RenderToTextureClass.h"
#include  "LightClass.h"
#include "ModelClass.h"
#include "TransparencyShaderClass.h"
#include "ShaderManagerClass.h"
#include "SpotLight.h"
#include "SpotLightSystem.h"
#include "BumpMapModel.h"
#include "BumpMapShaderClass.h"
#include "DebugWindowClass.h"
#include "StandardTextureClass.h"
#include "DepthShaderClass.h"
#include "ShadowShaderClass.h"
#include "ShadowLightClass.h"
#include "ParticleShaderSystem.h"
#include "ParticleSystem.h"
/////////////
// GLOBALS //
/////////////
 const  bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
//1.0f
const float SCREEN_NEAR = 1.0f;

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;
class GraphicsClass /* parasoft-suppress  OPT-13-DOWNGRADED "Not Included In Spec" */
{

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&) = default;
	GraphicsClass& operator=(const GraphicsClass&) = default;

	~GraphicsClass();
	bool Initialize(const HWND hwnd,const int ScreenWidth,const  int ScreenHeight);
	bool Frame() const;
	void IncrementZ() const;
	void DecrementZ() const;
	void IncrementY() const;
	void DecrementY() const;
	void Position1() const;
	void Position2() const;
	void IncrementX() const;
	void DecrementX() const;
	void SetWireFrame() const;
	void Cleanup();
	void YawPositive() const;
	void PitchPositive() const;
	void RollPositive() const;

	void YawNegative() const;
	void PitchNegative() const;
	void RollNegative() const;
private:
	bool RenderToTexture() const;
	bool Render() const;
	bool RenderScene() const;
	DirectXClass* m_Direct3D;
	CameraClass *Camera;
	TriangleClass *triangle;
	ShaderClass *shader;
	CubeClass*cube;
	TextureShaderClass* texture;
	TransparencyShaderClass* Transparent;
	LightClass* light;
	ModelClass* sphere;
	ModelClass* snowGlobe;
	ModelClass* Sub;
	ModelClass* SecondSub;
	ModelClass* Fish;

	BumpMapModel* BumpCube;

	RenderToTextureClass* m_RenderToTexture;
	ShaderManagerClass* m_ShaderManager;

	BumpMapShaderClass*  BumpMap;
	SpotLight* m_SpotLight;
	SpotLightSystem *m_Light1, *m_Light2, *m_Light3, *m_Light4;
	DebugWindowClass* m_DebugWindow;

	StandardTextureClass* S_Texture;
	Vector4* AmbientColor = new Vector4;;
	Vector4* SpecularColor;
	Vector4* DiffuseColor;
	Vector3* LightDir;
	Matrix world;
	Vector3* shadowLight;
	DepthShaderClass* m_depthShader;
	ShadowLightClass* m_ShadowLight;
	ShadowShaderClass* m_ShadowShader;

	Vector3* SpotLight1;
	Vector3* SpotLight2;
	Vector3* SpotLight3;
	Vector3* SpotLight4;
	Vector3* ShadowLightLocation;
	ModelClass* m_TropicalFish;
	ParticleSystem* m_ParticleSystem;
	ParticleShaderSystem* m_ParticleShader;
	ModelClass* Plane;
	BumpMapModel* StoneFish;
	ModelClass* SubFan;

};

