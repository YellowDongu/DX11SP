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

#include "MS01Terrain.h"
#include "OceanTile.h"

#include "MainCamera.h"
#include "SkyBox.h"

#include "TargetDummy.h"

#include "Player.h"
#include "F15E.h"
#include "TU160.h"

#include "WingVaporTrail.h"

#include "Trigger.h"
Player* player = nullptr;
F15E* enemyObject = nullptr;
TU160* testSuper = nullptr;
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
	ID3D11Device* dxDevice = DxDevice();
	ID3D11DeviceContext* dxDeviceContext = DxDeviceContext();

	EngineInstance()->ColliderSystem()->LoadNavMesh(L"../Bin/MS01.NvMsh");

	shader = Engine::DefaultModelShader::Create(dxDevice, dxDeviceContext);
	AddShader(shader->shaderFile, shader);
	SetShader(shader);

	//UIShader* uiShader = UIShader::Create(dxDevice, dxDeviceContext);
	//AddShader(L"DefaultUIShader.hlsl", uiShader);
	Engine::DefaultUIShader* newShader = Engine::DefaultUIShader::Create(dxDevice, dxDeviceContext);
	AddShader(newShader->shaderFile, newShader);

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
	testLayer->AddGameObject(L"MS01Terrain", MS01Terrain::Create(dxDevice, dxDeviceContext));
	OceanTile* oceanTile = OceanTile::Create(dxDevice, dxDeviceContext);
	oceanTile->transform()->Position() = Vector3(-2500.0f, -1.0f, -2500.0f);
	testLayer->AddGameObject(L"OceanTile", oceanTile);
	


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

	testSuper = TU160::Create(dxDevice, dxDeviceContext);
	testSuper->transform()->Position() = Vector3{ 5000.0f, ConvertFeetToWorld(10000.0f), 5000.0f };
	testSuper->Awake();
	//enemy->AddGameObject(L"MainTarget", testSuper);
	//bgm = Sound()->FindSound(L"ms01.wav");
	//Sound()->Play(bgmChannel, bgm, 160000.0f);

	Engine::Layer* particleLayer = Engine::Layer::Create();
	AddLayer(particleLayer, L"ParticleLayer");
	WingVaporTrail* wingVaporTrailSystem = WingVaporTrail::Create(dxDevice, dxDeviceContext);
	particleLayer->AddGameObject(L"WingVaporTrail", wingVaporTrailSystem);
	wingVaporTrailSystem->EnlistGameObject(player);
	wingVaporTrailSystem->EnlistGameObject(enemyObject);
	wingVaporTrailSystem->EnlistGameObject(testSuper);


	Engine::Layer* extra = Engine::Layer::Create();
	AddLayer(extra, L"ExtraLayer");
	TriggerBox* triggerBox = TriggerBox::Create(dxDevice, dxDeviceContext);
	extra->AddGameObject(L"TriggerBox", triggerBox);

	PositionSpawnTrigger::TriggerDescription description;
	GameObjectSpawnInfomation newSpawn;
	newSpawn.angle = Vector3::zero();
	newSpawn.position = testSuper->transform()->Position();
	newSpawn.layerName = L"MainTargetEnemy";
	newSpawn.name = L"MainTarget";
	newSpawn.gameObject = testSuper;
	description.spawnTargets.push_back(newSpawn);
	description.altitudeActive = false;
	description.position = player->transform()->Position() + Vector3{ 0.0f, 0.0f, ConvertFeetToWorld(4000.0f) * 5.0f };
	description.standByStatus = true;
	description.surveillanceTarget.push_back(player);
	description.triggerName = L"InitialTrigger";


	PositionSpawnTrigger* newTrigger = PositionSpawnTrigger::Create(description);
	triggerBox->AddTrigger(newTrigger);

}

void MS01::Awake(void)
{
	return;
	for (auto& layer : layers)
	{
		for (auto& gameObject : layer.second->GameObjectList())
		{
			gameObject.second->Awake();
		}
	}
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
