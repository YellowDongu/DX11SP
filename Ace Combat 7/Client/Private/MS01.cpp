#include "pch.h"
#include "MS01.h"


#include "AttitudeIndicator.h"
#include "FlightPathMarker.h"
#include "RaderUI.h"
#include "SpeedIndicator.h"
#include "AltitudeIndicator.h"
#include "Compass.h"
#include "PlayerStatusHUD.h"
#include "IFFHud.h"
#include "FCSHUD.h"

#include "DefaultModelShader.h"
#include "DefaultUIShader.h"
#include "UIShader.h"

#include "MS01Terrain.h"
#include "MainCamera.h"
#include "SkyBox.h"

#include "TargetDummy.h"

#include "Player.h"
#include "F15E.h"
Player* player = nullptr;
F15E* enemyObject = nullptr;

MS01::MS01(void)
{
}

void MS01::Free(void)
{
	End();
}

MS01* MS01::Create(void)
{
	MS01* newInstance = new MS01();
	return newInstance;
}
FMOD::Channel* bgmChannel;
FMOD::Sound* bgm;
void MS01::Start(void)
{
	// Awake¿¡´Ù°¡ ¹Ú¾Æ³ö¾ßµÊ
	//gameInstance->Device()->SetBackBufferColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	//meshTest = MeshTest::Create(gameInstance->Device()->GetDevice(), gameInstance->Device()->GetDeviceContext(), "D:\\PJ\\C++\\Class\\JusinDx3D\\JusinDx3DClient\\Bin\\Avatar_Girl_Bow_Fischl.fbx");
	//meshTest = MeshTest::Create(gameInstance->Device()->GetDevice(), gameInstance->Device()->GetDeviceContext(), "D:\\PJ\\C++\\Class\\JusinDx3D\\JusinDx3DClient\\Bin\\Keqing_Animation1.fbx");
	//meshTest = MeshTest::Create(gameInstance->Device()->GetDevice(), gameInstance->Device()->GetDeviceContext(), "D:\\PJ\\C++\\Class\\JusinDx3D\\JusinDx3DClient\\Bin\\TESTLAND.fbx");

	ID3D11Device* dxDevice = DxDevice();
	ID3D11DeviceContext* dxDeviceContext = DxDeviceContext();

	shader = Engine::DefaultModelShader::Create(dxDevice, dxDeviceContext);
	AddShader(shader->shaderFile, shader);
	SetShader(shader->shaderFile);

	//UIShader* uiShader = UIShader::Create(dxDevice, dxDeviceContext);
	//AddShader(L"DefaultUIShader.hlsl", uiShader);
	Engine::DefaultUIShader* newShader = Engine::DefaultUIShader::Create(dxDevice, dxDeviceContext);
	AddShader(newShader->shaderFile, newShader);

	//Engine::Model* testModel = Engine::Model::Create(dxDevice, dxDeviceContext, "..\\Bin\\f16c\\f16c.fbx");
	Engine::Layer* enemy = Engine::Layer::Create();
	AddLayer(enemy, L"Enemy");	
	enemy = Engine::Layer::Create();
	AddLayer(enemy, L"MainTargetEnemy");
	Engine::Layer* testLayer = Engine::Layer::Create();
	AddLayer(testLayer, L"TestLayer");
	enemyObject = F15E::Create(dxDevice, dxDeviceContext);
	player = Player::Create(dxDevice, dxDeviceContext);
	testLayer->AddGameObject(L"Player", player);
	enemy->AddGameObject(L"MainTargetEnemy", enemyObject);
	MainCamera* newInstance = MainCamera::Create(dxDevice, dxDeviceContext);
	player->LinkCamera(newInstance);
	testLayer->AddGameObject(L"MainCamera", newInstance);
	testLayer->AddGameObject(L"Terrain", MS01Terrain::Create(dxDevice, dxDeviceContext));
	testLayer->AddGameObject(L"SkyBox", SkyBox::Create(dxDevice, dxDeviceContext, newInstance));

	Engine::Layer* uiLayer = Engine::Layer::Create();
	AddLayer(uiLayer, L"UILayer");
	uiLayer->AddGameObject(L"AttitudeIndicator", AttitudeIndicator::Create(dxDevice, dxDeviceContext, player->transform()));
	uiLayer->AddGameObject(L"FlightPathMarker", FlightPathMarker::Create(dxDevice, dxDeviceContext, player));
	uiLayer->AddGameObject(L"Rader", RaderUI::Create(dxDevice, dxDeviceContext, player, nullptr, 0));
	uiLayer->AddGameObject(L"SpeedIndicator", SpeedIndicator::Create(dxDevice, dxDeviceContext, player));
	uiLayer->AddGameObject(L"AltitudeIndicator", AltitudeIndicator::Create(dxDevice, dxDeviceContext, player));
	uiLayer->AddGameObject(L"Compass", Compass::Create(dxDevice, dxDeviceContext, player));
	uiLayer->AddGameObject(L"PlayerStatusHUD", PlayerStatusHeadUpDisplay::Create(dxDevice, dxDeviceContext, player));
	uiLayer->AddGameObject(L"IFFHUD", IFFHUD::Create(dxDevice, dxDeviceContext, this, player, newInstance));
	uiLayer->AddGameObject(L"FCSHUD", FCSHUD::Create(dxDevice, dxDeviceContext, player, newInstance));

	Sound()->LoadSound("../Bin/Resources/Sounds/BGMs/","ms01.wav");
	//bgm = Sound()->FindSound(L"ms01.wav");
	//Sound()->Play(bgmChannel, bgm, 160000.0f);
}

void MS01::Awake(void)
{
}

void MS01::Update(void)
{
	//if (!Sound()->checkPlaying(bgmChannel))
		//Sound()->Play(bgmChannel, bgm, 31426.0f);
	Engine::Scene::Update();
}

void MS01::LateUpdate(void)
{
	Engine::Scene::LateUpdate();
}

void MS01::FixedUpdate(void)
{
	Engine::Scene::FixedUpdate();
}

void MS01::Render(void)
{
	SetShader(shader->shaderFile);
	GetCurrentShader()->PassNumber(0);
	Engine::Scene::Render();
}

void MS01::End(void)
{
	Engine::Scene::End();
}
