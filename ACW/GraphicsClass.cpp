#include "GraphicsClass.h"
#include <AntTweakBar.h>
GraphicsClass::GraphicsClass() : m_Direct3D(nullptr), Camera(nullptr), triangle(nullptr), shader(nullptr), cube(nullptr), texture(nullptr), Transparent(nullptr), light(nullptr), sphere(nullptr), snowGlobe(nullptr), Sub(nullptr),
                                 SecondSub(nullptr), Fish(nullptr), BumpCube(nullptr), m_RenderToTexture(nullptr), m_ShaderManager(nullptr), BumpMap(nullptr), m_SpotLight(nullptr), m_Light1(nullptr), m_Light2(nullptr), m_Light3(nullptr),
                                 m_Light4(nullptr), m_DebugWindow(nullptr), S_Texture(nullptr), AmbientColor(nullptr), SpecularColor(nullptr), DiffuseColor(nullptr), LightDir(nullptr), shadowLight(nullptr),
                                 m_depthShader(nullptr), m_ShadowLight(nullptr), m_ShadowShader(nullptr), SpotLight1(nullptr), SpotLight2(nullptr), SpotLight3(nullptr), SpotLight4(nullptr), m_TropicalFish(nullptr), m_ParticleSystem(nullptr), m_ParticleShader(nullptr), Plane(nullptr), StoneFish(nullptr), SubFan(nullptr)
{
	AmbientColor = new Vector4(0.50f, 0.50f, 0.50f, 1.0f);
	LightDir = new Vector3(0.0f, 0.0f, 1.0f);
	SpecularColor = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	DiffuseColor = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//location
	shadowLight = new Vector3(1.0f, -1.0f, 30.0f);
	//look at
	ShadowLightLocation = new Vector3(0.0f, 0.0, 40.0); // parasoft-suppress  INIT-14 "Not Included In Spec"
	SpotLight1 = new Vector3(3.0f, -10.0f, 30.0f);
	SpotLight2 = new Vector3(3.0, -10.0f, 30.0f);
	SpotLight3 = new Vector3(-3.0f, -1.0f, 30.0f);
	SpotLight4 = new Vector3(-3.0f, -1.0f, 30.0f);
}


GraphicsClass::~GraphicsClass()
{
	delete m_Direct3D;
	delete Camera;
	delete shader;
	delete cube;
	delete light;
	delete sphere;
	delete snowGlobe;
	delete Sub;
	delete SecondSub;
	delete Fish;
	delete BumpCube;
	delete m_ShaderManager;
	delete BumpMap;
	delete m_SpotLight;
	delete m_Light1;
	delete m_Light2;
	delete m_Light3;
	delete m_Light4;
	delete m_RenderToTexture;
	delete m_DebugWindow;
	delete S_Texture;
	delete AmbientColor;
	delete SpecularColor;
	delete DiffuseColor;
	delete LightDir;
	delete shadowLight;
	delete m_depthShader;
	delete m_ShadowLight;
	delete m_ShadowShader;
	delete SpotLight1;
	delete SpotLight2;
	delete SpotLight3;
	delete SpotLight4;
	delete m_ParticleSystem;
	delete m_ParticleShader;
	delete ShadowLightLocation;
	delete m_TropicalFish;
	delete Plane;
	delete StoneFish;
	delete SubFan;
}

bool GraphicsClass::Initialize(const HWND hwnd,const int ScreenWidth,const int ScreenHeight)
{
	// Create the Direct3D object.
	m_Direct3D = new DirectXClass;
	// Initialize the Direct3D object.
	m_Direct3D->DirectXInitialize(hwnd, ScreenWidth, ScreenHeight, FULL_SCREEN);


	 Camera = new CameraClass;
	 // Set the initial position of the camera.
	 Camera->cameraSetup(ScreenWidth, ScreenHeight);
	 Camera->SetPosition(0.0f, 0.0f, -10.0f);
	 
	 /*Camera->SetRotation(0.0f, 5.0f, 0.0f);*/
	const char* const spherefilename = "../ACW/data/Models/sphere.txt";
	sphere = new ModelClass;
	sphere->InitializeTexture(m_Direct3D->GetDevice(), spherefilename, L"../ACW/data/rust.dds");

	m_TropicalFish = new ModelClass;
	m_TropicalFish->InitializeTexture(m_Direct3D->GetDevice(), "../ACW/data/Models/tropical.txt", L"../ACW/data/rust.dds");

	snowGlobe = new ModelClass;
	snowGlobe->InitializeTexture(m_Direct3D->GetDevice(), spherefilename, L"../ACW/data/glass.dds");

	Sub = new ModelClass;
	Sub->InitializeTexture(m_Direct3D->GetDevice(), "../ACW/data/Models/model.txt", L"../ACW/data/metal.dds");

	SecondSub = new ModelClass;
	SecondSub->InitializeTexture(m_Direct3D->GetDevice(), "../ACW/data/Models/sub2.txt", L"../ACW/data/rust.dds");

	S_Texture = new StandardTextureClass;
	S_Texture->Initialize(m_Direct3D->GetDevice());

	Fish = new ModelClass;
	Fish->InitializeTexture(m_Direct3D->GetDevice(), "../ACW/data/Models/fish.txt", L"../ACW/data/shark.dds");

	m_ShaderManager = new ShaderManagerClass;
	m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);

	BumpMap = new BumpMapShaderClass;
	BumpMap->Initialize(m_Direct3D->GetDevice());

	Plane = new ModelClass;
	Plane->InitializeTexture(m_Direct3D->GetDevice(), "../ACW/data/Models/plane.txt", L"../ACW/data/shark.dds");

	BumpCube = new BumpMapModel;
	BumpCube->Initialize(m_Direct3D->GetDevice(), "../ACW/data/Models/cube.txt", L"../ACW/data/box.dds", L"../ACW/data/bump01.dds");


	cube = new CubeClass;
	cube->Initialize(m_Direct3D->GetDevice(), "../ACW/data/Models/cube.txt", L"../ACW/data/seafloor.dds");

	m_ShadowShader = new ShadowShaderClass;
	m_ShadowShader->Initialize(m_Direct3D->GetDevice());


	m_depthShader = new DepthShaderClass;
	m_depthShader->Initialize(m_Direct3D->GetDevice());

	m_RenderToTexture = new RenderToTextureClass;
	m_RenderToTexture->Initialize(m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);

	m_DebugWindow = new DebugWindowClass;
	m_DebugWindow->Initialize(m_Direct3D->GetDevice(), ScreenWidth, ScreenHeight, 100,100);

	// Create the particle shader object.
	m_ParticleShader = new ParticleShaderSystem;
	if (!m_ParticleShader)
	{
		return false;
	}

	// Initialize the particle shader object.
	 m_ParticleShader->Initialize(m_Direct3D->GetDevice());

	// Create the particle system object.
	m_ParticleSystem = new ParticleSystem;
	if (!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	m_ParticleSystem->Initialize(m_Direct3D->GetDevice(), L"../ACW/data/bubble.dds");
	StoneFish = new BumpMapModel;
	StoneFish->Initialize(m_Direct3D->GetDevice(), "../ACW/data/Models/stoneFish.txt", L"../ACW/data/StoneFish.dds", L"../ACW/data/Fish_Bump.dds");

	SubFan = new ModelClass;
	SubFan->InitializeTexture(m_Direct3D->GetDevice(), "../ACW/data/Models/subfan.txt", L"../ACW/data/metal.dds");
#pragma region Antweak
	////initialize anttweak
	TwInit(TW_DIRECT3D11, m_Direct3D->GetDevice());

	TwWindowSize(ScreenWidth, ScreenHeight);
	
	TwBar *myBar;
	myBar = TwNewBar("ACW VARIABLES");
	
	TwAddVarRW(myBar, "AmbientColor", TW_TYPE_COLOR4F, AmbientColor, "ColorMode=RGB");
	TwAddVarRW(myBar, "LightDirection", TW_TYPE_DIR3F, LightDir, "label");
	TwAddVarRW(myBar, "SpecularColor", TW_TYPE_COLOR4F, SpecularColor, "label");
	TwAddVarRW(myBar, "Diffuse", TW_TYPE_COLOR4F, DiffuseColor, "ColorMode=RBGA");
	TwAddVarRW(myBar, "ShadowLLocation", TW_TYPE_DIR3F, shadowLight, "label");
	TwAddVarRW(myBar, "ShadowLLookAt", TW_TYPE_DIR3F, ShadowLightLocation, "label");
	TwAddVarRW(myBar, "SpotLightP1", TW_TYPE_DIR3F, SpotLight1, "");
	TwAddVarRW(myBar, "SpotLightP2", TW_TYPE_DIR3F, SpotLight2, "");
	TwAddVarRW(myBar, "SpotLightP3", TW_TYPE_DIR3F, SpotLight3, "");
	TwAddVarRW(myBar, "SpotLightP4", TW_TYPE_DIR3F, SpotLight4, "");
#pragma endregion 
#pragma region Lights
	// Initialize the light object.
	// Create the light object.
	light = new LightClass;


	m_SpotLight = new SpotLight;
	m_SpotLight->Initialize(m_Direct3D->GetDevice());

	m_Light1 = new SpotLightSystem;
	// Initialize the first light object.
	m_Light1->SetDiffuseColor(0.0f, 0.0f, 0.0f, 0.0f);


	// Create the second light object.
	m_Light2 = new SpotLightSystem;
	if (!m_Light2)
	{
		return false;
	}

	// Initialize the second light object.
	m_Light2->SetDiffuseColor(0.0f, 1.0f, 0.0f, 0.3f);


	// Create the third light object.
	m_Light3 = new SpotLightSystem;
	if (!m_Light3)
	{
		return false;
	}

	// Initialize the third light object.
	m_Light3->SetDiffuseColor(1.0f, 0.0f, 0.5f, 0.0f);


	// Create the fourth light object.
	m_Light4 = new SpotLightSystem;
	// Initialize the fourth light object.
	m_Light4->SetDiffuseColor(1.0f, 0.4f, 1.0f, 0.5f);
	
	if (!m_Light4)
	{
		return false;
	}
	//here
	m_ShadowLight = new ShadowLightClass;
	m_ShadowLight->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_ShadowLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);


#pragma endregion 
	return true;
}

bool GraphicsClass::Frame() const
{
	bool result;
	// Update the position of the light each frame.

	// Update the position of the light.

	Camera->Render();
	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

void GraphicsClass::IncrementZ() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->IncrementZAxis();
}


//
void GraphicsClass::DecrementZ() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->DecrementZAxis();
}

void GraphicsClass::IncrementY() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->IncrementYAxis();
}

//
void GraphicsClass::DecrementY() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->DecrementYAxis();
}

void GraphicsClass::Position1() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->Position1();
}

void GraphicsClass::Position2() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->Position2();
}

void GraphicsClass::IncrementX() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->IncrementXAxis();
}
//
void GraphicsClass::DecrementX() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->DecrementXAxis();
}

void GraphicsClass::SetWireFrame() const
{
	static bool state = false;
	
	if(state == false)
	{
		m_Direct3D->SetFill();
		
		state = true;
	}
	else if (state == true)
	{
		m_Direct3D->SetWireFrame();
		state = false;
	}
	
}

void GraphicsClass::Cleanup()
{
	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Cleanup();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}
}

void GraphicsClass::YawPositive() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->YawPositive();
}

void GraphicsClass::PitchPositive() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->PitchPositive();
}

void GraphicsClass::RollPositive() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->RollPositive();
}

void GraphicsClass::YawNegative() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->YawNegative();
}

void GraphicsClass::PitchNegative() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->PitchNegative();
}

void GraphicsClass::RollNegative() const // parasoft-suppress  OPT-24 "Not Included In Spec"
{
	Camera->RollNegative();
}

bool GraphicsClass::RenderToTexture() const
{
	const XMMATRIX mTranslate = DirectX::XMMatrixTranslation(0.0f, -12.5f, 20.0f);
	const XMMATRIX snowglobescale = DirectX::XMMatrixScaling(15.0, 15.0f, 15.0f);
	//resize everything and fix lights
	const XMMATRIX mTranslate2 = DirectX::XMMatrixTranslation(0.0f, 0.0f, 50.0f);
	const XMMATRIX mscale = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
	const XMMATRIX mscale2 = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
	/*const XMMATRIX mSpin2 = DirectX::XMMatrixRotationZ(t);*/


	//const XMMATRIX mOrbit2 = DirectX::XMMatrixRotationY(t * 1.0f);
	const XMMATRIX mTranslate3 = DirectX::XMMatrixTranslation(0.0f, 0.0f, 45.0f);//*DirectX::XMMatrixRotationY(t); ;
	const XMMATRIX SecondSubTranslate = DirectX::XMMatrixTranslation(0.0f, 15.0f, 45.0f);
	const XMMATRIX Scaling = DirectX::XMMatrixScaling(0.001f, 0.001f, 0.001f);
	const XMMATRIX TreasureScaling = DirectX::XMMatrixScaling(3.9f, 2.0f, 2.0f);
	const XMMATRIX TreasureChestWorld = mTranslate * TreasureScaling;//*// mOrbit2;
	const XMMATRIX subworld = mscale * mTranslate2;// *DirectX::XMMatrixRotationY(t);
	const XMMATRIX fishTranslate = DirectX::XMMatrixTranslation(0.0f, -15.5f, 30.0f);
	const XMMATRIX secondsubworld = mscale2 *  SecondSubTranslate; //*DirectX::XMMatrixRotationY(t);
	const XMMATRIX PebbleWorld = DirectX::XMMatrixTranslation(-9.0f, -40.0f, 55.0f) * DirectX::XMMatrixScaling(0.6f, 0.6f, 0.6f);
	const XMMATRIX fishworld = Scaling *  fishTranslate;
	const XMMATRIX tropicalfishworld = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f) *  DirectX::XMMatrixTranslation(0.0f, 0.0f, 35.0f); //* XMMatrixRotationY(t);
	//const XMMATRIX SGworld = snowglobescale * mTranslate3;
	//const XMMATRIX shadowtranslate = DirectX::XMMatrixTranslation(0.0f, 0.0f, 40.0f); //* DirectX::XMMatrixScaling(3.0f, 3.0f, 3.0f);
	//const XMMATRIX particleworld = DirectX::XMMatrixTranslation(0.0f, -25.5f, 40.0f);// *DirectX::XMMatrixRotationX(45);
	//																				 //const XMMATRIX discoball = DirectX::XMMatrixTranslation(-9.0f, -25.0f, 45.0f);
	//const XMMATRIX PlaneWorld = DirectX::XMMatrixTranslation(0.0f, -2.0f, 40.0f);

	Matrix worldMatrix, lightViewMatrix, lightProjectionMatrix;

	//// Set the render target to be the render to texture.
	m_RenderToTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());
	// Clear the render to texture.
	m_RenderToTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_ShadowLight->GenerateViewMatrix();
	Camera->GetWorldMatrix(worldMatrix);
	m_ShadowLight->GetViewMatrix(lightViewMatrix);
	m_ShadowLight->GetProjectionMatrix(lightProjectionMatrix);

	sphere->Render(m_Direct3D->GetDeviceContext());
	m_depthShader->Render(m_Direct3D->GetDeviceContext(), sphere->GetIndexCount(), PebbleWorld, lightViewMatrix, lightProjectionMatrix);

	m_TropicalFish->Render(m_Direct3D->GetDeviceContext());
	m_depthShader->Render(m_Direct3D->GetDeviceContext(), m_TropicalFish->GetIndexCount(), tropicalfishworld, lightViewMatrix, lightProjectionMatrix);

	Sub->Render(m_Direct3D->GetDeviceContext());
	m_depthShader->Render(m_Direct3D->GetDeviceContext(), Sub->GetIndexCount(), subworld, lightViewMatrix, lightProjectionMatrix);

	SecondSub->Render(m_Direct3D->GetDeviceContext());
	m_depthShader->Render(m_Direct3D->GetDeviceContext(), SecondSub->GetIndexCount(), secondsubworld, lightViewMatrix, lightProjectionMatrix);

	Fish->Render(m_Direct3D->GetDeviceContext());
	m_depthShader->Render(m_Direct3D->GetDeviceContext(), Fish->GetIndexCount(), fishworld, lightViewMatrix, lightProjectionMatrix);


	BumpCube->Render(m_Direct3D->GetDeviceContext());
	m_depthShader->Render(m_Direct3D->GetDeviceContext(), BumpCube->GetIndexCount(), TreasureChestWorld, lightViewMatrix, lightProjectionMatrix);

	//cube->Render(m_Direct3D->GetDeviceContext());
	//m_depthShader->Render(m_Direct3D->GetDeviceContext(), cube->GetIndexCount(), shadowtranslate, lightViewMatrix, lightProjectionMatrix);


	//Plane->Render(m_Direct3D->GetDeviceContext());
	//m_depthShader->Render(m_Direct3D->GetDeviceContext(), Plane->GetIndexCount(), PlaneWorld, lightViewMatrix, lightProjectionMatrix);


	//snowGlobe->Render(m_Direct3D->GetDeviceContext());			//50
	//m_depthShader->Render(m_Direct3D->GetDeviceContext(), snowGlobe->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);


	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	 m_Direct3D->SetBackBufferRenderTarget();
	 m_Direct3D->ResetViewPort();
	return true;
}

bool GraphicsClass::Render() const
{
	m_ShadowLight->SetPosition(shadowLight->x, shadowLight->y, shadowLight->z);
	m_ShadowLight->SetLookAt(ShadowLightLocation->x, ShadowLightLocation->y, ShadowLightLocation->z);

	m_ShadowLight->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);
	/*bool result;*/
	RenderToTexture();

	m_Direct3D->BeginScene();
	RenderScene();
	////2d rendering
	////m_Direct3D->TurnZBufferOff();
	//m_Direct3D->GetWorldMatrix(worldMatrix);
	//Camera->GetViewMatrix(viewMatrix);
	//m_Direct3D->GetOrthoMatrix(orthoMatrix);
	//m_DebugWindow->Render(m_Direct3D->GetDeviceContext(), 50, 50);

	//m_ShaderManager->RenderStandardTexture(m_Direct3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix,
	//	orthoMatrix, m_RenderToTexture->GetShaderResourceView());
	/*m_Direct3D->TurnZBufferOn();*/
	TwDraw();
	m_Direct3D->EndScene();
	return true;
}


bool GraphicsClass::RenderScene() const
{
	Matrix worldMatrix, viewMatrix, projectionMatrix;
	Matrix lightViewMatrix, lightProjectionMatrix;

	Vector4 diffuseColor[4];
	Vector4 lightPosition[4];


	light->SetAmbientColor(AmbientColor->x, AmbientColor->y, AmbientColor->z, AmbientColor->w);
	light->SetDiffuseColor(DiffuseColor->x, DiffuseColor->y, DiffuseColor->z, DiffuseColor->w);
	light->SetDirection(LightDir->x, LightDir->y, LightDir->z);
	light->SetSpecularColor(SpecularColor->x, SpecularColor->y, SpecularColor->z, SpecularColor->w);
	light->SetSpecularPower(32.0f);
	m_Light1->SetPosition(SpotLight1->x, SpotLight1->y, SpotLight1->z);
	m_Light2->SetPosition(SpotLight2->x, SpotLight2->y, SpotLight2->z);
	m_Light3->SetPosition(SpotLight3->x, SpotLight3->y, SpotLight3->z);
	m_Light4->SetPosition(SpotLight4->x, SpotLight4->y, SpotLight4->z);
		static float t = 0.0f;
		static ULONGLONG timeStart = 0;
		const	ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 1000.0f;
	
		float blendAmount;
		// Set the blending amount to 50%.
		blendAmount = 0.50f;
	
	const XMMATRIX mTranslate = DirectX::XMMatrixTranslation(0.0f, -12.5f, 20.0f);
	const XMMATRIX snowglobescale = DirectX::XMMatrixScaling(15.0, 15.0f, 15.0f);
	//resize everything and fix lights
	const XMMATRIX mTranslate2 = DirectX::XMMatrixTranslation(0.0f, 0.0f, 50.0f);
	const XMMATRIX mscale = DirectX::XMMatrixScaling(0.01f,0.01f, 0.01f);
	const XMMATRIX mscale2 = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
	/*const XMMATRIX mSpin2 = DirectX::XMMatrixRotationZ(t);*/


	//const XMMATRIX mOrbit2 = DirectX::XMMatrixRotationY(t * 1.0f);
	const XMMATRIX mTranslate3 = DirectX::XMMatrixTranslation(0.0f, 0.0f, 45.0f);//*DirectX::XMMatrixRotationY(t); ;
	const XMMATRIX SecondSubTranslate = DirectX::XMMatrixTranslation(0.0f, 15.0f, 45.0f);
	const XMMATRIX Scaling = DirectX::XMMatrixScaling(0.001f, 0.001f, 0.001f);
	const XMMATRIX TreasureScaling = DirectX::XMMatrixScaling(3.9f, 2.0f, 2.0f);
	const XMMATRIX TreasureChestWorld = mTranslate * TreasureScaling;//*// mOrbit2;
	const XMMATRIX subworld = mscale * mTranslate2;// *DirectX::XMMatrixRotationY(t);
	const XMMATRIX fishTranslate = DirectX::XMMatrixTranslation(0.0f, -15.5f, 30.0f);
	const XMMATRIX secondsubworld = mscale2 *  SecondSubTranslate; //*DirectX::XMMatrixRotationY(t);
	const XMMATRIX PebbleWorld = DirectX::XMMatrixTranslation(-9.0f, -40.0f, 55.0f) * DirectX::XMMatrixScaling(0.6f, 0.6f, 0.6f);
	const XMMATRIX fishworld = Scaling *  fishTranslate;
	const XMMATRIX tropicalfishworld = DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f) *  DirectX::XMMatrixTranslation(0.0f, 0.0f, 35.0f); //* XMMatrixRotationY(t);
	const XMMATRIX SGworld = snowglobescale * mTranslate3;
	//const XMMATRIX shadowtranslate = DirectX::XMMatrixTranslation(0.0f, 0.0f, 40.0f); //* DirectX::XMMatrixScaling(3.0f, 3.0f, 3.0f);
	const XMMATRIX particleworld = DirectX::XMMatrixTranslation(0.0f, -25.5f, 40.0f);// *DirectX::XMMatrixRotationX(45);
	//const XMMATRIX discoball = DirectX::XMMatrixTranslation(-9.0f, -25.0f, 45.0f);
	const XMMATRIX StoneFishWorld = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f)  * DirectX::XMMatrixTranslation(0.0f, -40.0f, 50.0f);
	const XMMATRIX SubFanWorld = XMMatrixRotationX(t) *  DirectX::XMMatrixTranslation(-6.0f, 0.0f, 50.0f) * DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);

	// Clear the buffers to begin the scene.

	// Create the diffuse color array from the four light colors.
	diffuseColor[0] = m_Light1->GetDiffuseColor();
	diffuseColor[1] = m_Light2->GetDiffuseColor();
	diffuseColor[2] = m_Light3->GetDiffuseColor();
	diffuseColor[3] = m_Light4->GetDiffuseColor();

	// Create the light position array from the four light positions.
	lightPosition[0] = m_Light1->GetPosition();
	lightPosition[1] = m_Light2->GetPosition();
	lightPosition[2] = m_Light3->GetPosition();
	lightPosition[3] = m_Light4->GetPosition();


	Camera->Render();
	m_ShadowLight->GenerateViewMatrix();
	Camera->GetWorldMatrix(worldMatrix);
	Camera->GetViewMatrix(viewMatrix);
	Camera->GetProjectionMatrix(projectionMatrix);

	m_ShadowLight->GetViewMatrix(lightViewMatrix);
	m_ShadowLight->GetProjectionMatrix(lightProjectionMatrix);


	sphere->Render(m_Direct3D->GetDeviceContext());
	m_ShaderManager->RenderSpotLightShader(m_Direct3D->GetDeviceContext(), sphere->GetIndexCount(), PebbleWorld, viewMatrix, projectionMatrix,
	                                       sphere->GetTexture(), diffuseColor, lightPosition);
	m_TropicalFish->Render(m_Direct3D->GetDeviceContext());
	m_ShaderManager->RenderStandardTexture(m_Direct3D->GetDeviceContext(), m_TropicalFish->GetIndexCount(), tropicalfishworld, viewMatrix, projectionMatrix, m_TropicalFish->GetTexture());

	////FIRST SUB RENDER
	Sub->Render(m_Direct3D->GetDeviceContext());
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), Sub->GetIndexCount(), subworld, viewMatrix, projectionMatrix, lightViewMatrix,
		lightProjectionMatrix, Sub->GetTexture(), m_RenderToTexture->GetShaderResourceView(), m_ShadowLight->GetPosition(),
		m_ShadowLight->GetAmbientColor(), m_ShadowLight->GetDiffuseColor());//////////

	SubFan->Render(m_Direct3D->GetDeviceContext());
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), SubFan->GetIndexCount(), SubFanWorld, viewMatrix, projectionMatrix, lightViewMatrix,
		lightProjectionMatrix, SubFan->GetTexture(), m_RenderToTexture->GetShaderResourceView(), m_ShadowLight->GetPosition(),
		m_ShadowLight->GetAmbientColor(), m_ShadowLight->GetDiffuseColor());//////////
	
	////SecondSub Render
	SecondSub->Render(m_Direct3D->GetDeviceContext());
	m_ShaderManager->RenderLightShader(m_Direct3D->GetDeviceContext(), SecondSub->GetIndexCount(), secondsubworld,viewMatrix,projectionMatrix, SecondSub->GetTexture(), light->GetDirection(), light->GetDiffuseColor(), light->GetAmbientColor(), Camera->GetPosition(), light->GetSpecularColor(), light->GetSpecularPower());
	/////////

	///////FISH RENDER
	Fish->Render(m_Direct3D->GetDeviceContext());
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), Fish->GetIndexCount(), fishworld, viewMatrix, projectionMatrix, lightViewMatrix,
		lightProjectionMatrix, cube->GetTexture(), m_RenderToTexture->GetShaderResourceView(), m_ShadowLight->GetPosition(),
		m_ShadowLight->GetAmbientColor(), m_ShadowLight->GetDiffuseColor());
	//m_ShaderManager->RenderLightShader(m_Direct3D->GetDeviceContext(), Fish->GetIndexCount(), fishworld, viewMatrix, projectionMatrix, Fish->GetTexture(), light->GetDirection(), light->GetDiffuseColor(), light->GetAmbientColor(), Camera->GetPosition(), light->GetSpecularColor(), light->GetSpecularPower());
	////////////////

	BumpCube->Render(m_Direct3D->GetDeviceContext());
	BumpMap->Render(m_Direct3D->GetDeviceContext(), BumpCube->GetIndexCount(), TreasureChestWorld, viewMatrix, projectionMatrix,
	BumpCube->GetTextureArray(), light->GetDirection(), light->GetDiffuseColor());
	//
	StoneFish->Render(m_Direct3D->GetDeviceContext());
	BumpMap->Render(m_Direct3D->GetDeviceContext(), StoneFish->GetIndexCount(), StoneFishWorld, viewMatrix, projectionMatrix,
		StoneFish->GetTextureArray(), light->GetDirection(), light->GetDiffuseColor());
	//Plane->Render(m_Direct3D->GetDeviceContext());
	//m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), Plane->GetIndexCount(), PlaneWorld, viewMatrix, projectionMatrix, lightViewMatrix,
	//	lightProjectionMatrix, cube->GetTexture(), m_RenderToTexture->GetShaderResourceView(), m_ShadowLight->GetPosition(),
	//	m_ShadowLight->GetAmbientColor(), m_ShadowLight->GetDiffuseColor());

	//cube->Render(m_Direct3D->GetDeviceContext());
	//m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), cube->GetIndexCount(), shadowtranslate, viewMatrix, projectionMatrix, lightViewMatrix,
	//                       lightProjectionMatrix, cube->GetTexture(), m_RenderToTexture->GetShaderResourceView(), m_ShadowLight->GetPosition(),
	//                       m_ShadowLight->GetAmbientColor(), m_ShadowLight->GetDiffuseColor());

	m_Direct3D->TurnOnParticleBlend();
	// Run the frame processing for the particle system.
	m_ParticleSystem->Frame(t, m_Direct3D->GetDeviceContext());

	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the texture shader.
	m_ParticleShader->Render(m_Direct3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), particleworld, viewMatrix, projectionMatrix,
		m_ParticleSystem->GetTexture());
	m_ParticleShader->Render(m_Direct3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), SubFanWorld, viewMatrix, projectionMatrix,
		m_ParticleSystem->GetTexture());
	//////////SNOWGLOBE
	m_Direct3D->TurnOnAlphaBlending();
	snowGlobe->Render(m_Direct3D->GetDeviceContext());			//50
	m_ShaderManager->RenderTransparencyShader(m_Direct3D->GetDeviceContext(), snowGlobe->GetIndexCount(), SGworld,viewMatrix, projectionMatrix, snowGlobe->GetTexture(), 0.10);
	m_Direct3D->TurnOffAlphaBlending();


	return true;
}
