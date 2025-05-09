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
#include "SurfaceToAirMissileTruck.h"

#include "WingVaporTrail.h"
#include "MissileTrail.h"

#include "Trigger.h"
#include "AIPilot.h"

#include "Explosion.h"
Player* player = nullptr;
F15E* enemyObject = nullptr;
TU160* testSuper = nullptr;
SAM* samTest = nullptr;
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
	ID3D11Device* dxDevice = DxDevice();
	ID3D11DeviceContext* dxDeviceContext = DxDeviceContext();

	EngineInstance()->ColliderSystem()->LoadNavMesh(L"../Bin/MS01.NvMsh");

	shader = Engine::DefaultModelShader::Create(dxDevice, dxDeviceContext);
	AddShader(shader->shaderFile, shader);

	Engine::Layer* particleLayer = Engine::Layer::Create();
	AddLayer(particleLayer, L"ParticleLayer");
	WingVaporTrail* wingVaporTrailSystem = WingVaporTrail::Create(dxDevice, dxDeviceContext);
	particleLayer->AddGameObject(L"WingVaporTrail", wingVaporTrailSystem);
	MissileTrail* missileTrailSystem = MissileTrail::Create(dxDevice, dxDeviceContext);
	missileTrailSystem->Awake();
	particleLayer->AddGameObject(L"MissileTrail", missileTrailSystem);

	Explosion* explosion = Explosion::Create(dxDevice, dxDeviceContext);
	explosion->Awake();
	particleLayer->AddGameObject(L"Explosion", explosion);


	Engine::Layer* MainTargetEnemy = Engine::Layer::Create();
	AddLayer(MainTargetEnemy, L"MainTargetEnemy");
	enemyObject = F15E::Create(dxDevice, dxDeviceContext);
	MainTargetEnemy->AddGameObject(L"MainTargetEnemy", enemyObject);
	enemyObject->Awake();

	Engine::Layer* MainTargetEnemyGround = nullptr;

	Engine::Layer* Enemy = Engine::Layer::Create();
	AddLayer(Enemy, L"Enemy");
	Enemy->AddGameObject(L"TargetDummy1", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 500.0f,  50.0f, 500.0f }));
	Enemy->AddGameObject(L"TargetDummy2", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 1000.0f, 50.0f, 1000.0f }));
	Enemy->AddGameObject(L"TargetDummy3", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 1000.0f, 25.0f, 1000.0f }));
	Enemy->AddGameObject(L"TargetDummy4", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 900.0f,  25.0f, 900.0f }));
	Enemy->AddGameObject(L"TargetDummy5", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 1000.0f, 50.0f, 500.0f }));
	Enemy->AddGameObject(L"TargetDummy6", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 500.0f,  50.0f, 1000.0f }));
	Enemy->AddGameObject(L"TargetDummy7", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 2000.0f, 50.0f, 2000.0f }));
	Enemy->AddGameObject(L"TargetDummy8", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 2000.0f, 25.0f, 2000.0f }));
	Enemy->AddGameObject(L"TargetDummy9", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 2000.0f, 50.0f, 1000.0f }));
	Enemy->AddGameObject(L"TargetDummy10", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 1000.0f, 50.0f, 50.0f }));
	Enemy->AddGameObject(L"TargetDummy11", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 1000.0f, 50.0f, 200.0f }));
	Enemy->AddGameObject(L"TargetDummy12", TargetDummy::Create(dxDevice, dxDeviceContext, Vector3{ 1000.0f, 50.0f, 100.0f }));

	//Engine::Layer* GroundEnemy = nullptr;
	//Engine::Layer* NonTargetEnemy = nullptr;
	//Engine::Layer* NonTargetGroundEnemy = nullptr;
	//Engine::Layer* MainTargetAlly = nullptr;
	//Engine::Layer* MainTargetAllyGround = nullptr;

	Engine::Layer* Ally = Engine::Layer::Create();
	AddLayer(Ally, L"Ally");
	player = Player::Create(dxDevice, dxDeviceContext);
	player->Awake();
	Ally->AddGameObject(L"Player", player);

	//Engine::Layer* GroundAlly = nullptr;
	//Engine::Layer* NonTargetAlly = nullptr;
	//Engine::Layer* NonTargetGroundAlly = nullptr;
	//Engine::Layer* UnKnown = nullptr;
	//Engine::Layer* GroundUnKnown = nullptr;
	samTest = SAM::Create(dxDevice, dxDeviceContext);
	samTest->Awake();
	samTest->transform()->Position() = Vector3{ 2000.0f, 0.0f, 3000.0f };

	Engine::NavMesh* navMesh = EngineInstance()->ColliderSystem()->CurrentNavMesh();
	INT triangleIndex = navMesh->GetTriangleIndex(samTest->transform()->Position());
	if (triangleIndex != -1)
	{
		FLOAT y = navMesh->GetNode(triangleIndex)->getHeight(samTest->transform()->Position());
		samTest->transform()->Position().y = y;
	}
	MainTargetEnemy->AddGameObject(L"SAM", samTest);


	//UIShader* uiShader = UIShader::Create(dxDevice, dxDeviceContext);
	//AddShader(L"DefaultUIShader.hlsl", uiShader);
	//Engine::DefaultUIShader* newShader = Engine::DefaultUIShader::Create(dxDevice, dxDeviceContext);
	//AddShader(newShader->shaderFile, newShader);

	Engine::Layer* enviormentLayer = Engine::Layer::Create();
	AddLayer(enviormentLayer, L"TestLayer");

	MainCamera* newInstance = MainCamera::Create(dxDevice, dxDeviceContext);
	enviormentLayer->AddGameObject(L"MainCamera", newInstance);
	player->LinkCamera(newInstance);

	enviormentLayer->AddGameObject(L"MS01Terrain", MS01Terrain::Create(dxDevice, dxDeviceContext));

	//OceanTile* oceanTile = OceanTile::Create(dxDevice, dxDeviceContext);
	//oceanTile->transform()->Position() = Vector3(-2500.0f, -1.0f, -2500.0f);
	//enviormentLayer->AddGameObject(L"OceanTile", oceanTile);

	OceanTile* oceanTile = OceanTile::Create(dxDevice, dxDeviceContext);
	oceanTile->transform()->Scale() = Vector3::one() * 0.05f;
	Engine::GameObject* oceanTileClone = oceanTile->Clone();

	oceanTile->transform()->Position() = Vector3(500.0f, 0.0f, 0.0f);
	enviormentLayer->AddGameObject(L"OceanTile", oceanTile);

	oceanTileClone->transform()->Position() = Vector3(0.0f, 0.0f, 500.0f);
	enviormentLayer->AddGameObject(L"OceanTile", oceanTileClone);

	oceanTileClone = oceanTile->Clone();
	oceanTileClone->transform()->Position() = Vector3(500.0f, 0.0f, 500.0f);
	enviormentLayer->AddGameObject(L"OceanTile", oceanTileClone);

	oceanTileClone = oceanTile->Clone();
	oceanTileClone->transform()->Position() = Vector3(0.0f, 0.0f, 1000.0f);
	enviormentLayer->AddGameObject(L"OceanTile", oceanTileClone);

	oceanTileClone = oceanTile->Clone();
	oceanTileClone->transform()->Position() = Vector3(500.0f, 0.0f, 1000.0f);
	enviormentLayer->AddGameObject(L"OceanTile", oceanTileClone);

	oceanTileClone = oceanTile->Clone();
	oceanTileClone->transform()->Position() = Vector3(1000.0f, 0.0f, 1000.0f);
	enviormentLayer->AddGameObject(L"OceanTile", oceanTileClone);

	oceanTileClone = oceanTile->Clone();
	oceanTileClone->transform()->Position() = Vector3(500.0f, 0.0f, 1500.0f);
	enviormentLayer->AddGameObject(L"OceanTile", oceanTileClone);

	oceanTileClone = oceanTile->Clone();
	oceanTileClone->transform()->Position() = Vector3(1000.0f, 0.0f, 1500.0f);
	enviormentLayer->AddGameObject(L"OceanTile", oceanTileClone);

	oceanTileClone = oceanTile->Clone();
	oceanTileClone->transform()->Position() = Vector3(0.0f, 0.0f, 2000.0f);
	enviormentLayer->AddGameObject(L"OceanTile", oceanTileClone);

	oceanTileClone = oceanTile->Clone();
	oceanTileClone->transform()->Position() = Vector3(0.0f, 0.0f, 2500.0f);
	enviormentLayer->AddGameObject(L"OceanTile", oceanTileClone);
	FLOAT z = 0.0f;
	FLOAT x = 2000.0f;
	for (FLOAT i = 0; i < 6; i++)
	{
		z = 500.0f * i;


		oceanTileClone = oceanTile->Clone();
		oceanTileClone->transform()->Position() = Vector3(x, 0.0f, z);
		enviormentLayer->AddGameObject(L"OceanTile", oceanTileClone);

	}



	//oceanTile->transform()->Position() = Vector3(-2500.0f, -1.0f, -2500.0f);
	//enviormentLayer->AddGameObject(L"OceanTile", oceanTile);



	enviormentLayer->AddGameObject(L"SkyBox", SkyBox::Create(dxDevice, dxDeviceContext, newInstance));


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


	testSuper = TU160::Create(dxDevice, dxDeviceContext);
	testSuper->transform()->Position() = Vector3{ 5000.0f, ConvertFeetToWorld(10000.0f), 5000.0f };
	//enemy->AddGameObject(L"MainTarget", testSuper);
	//bgm = Sound()->FindSound(L"ms01.wav");
	//Sound()->Play(bgmChannel, bgm, 160000.0f);


	Engine::Layer* extra = Engine::Layer::Create();
	AddLayer(extra, L"ExtraLayer");
	TriggerBox* triggerBox = TriggerBox::Create(dxDevice, dxDeviceContext);
	extra->AddGameObject(L"TriggerBox", triggerBox);


	KillSpawnTrigger::TriggerDescription killTriggerDescription;
	GameObjectSpawnInfomation newSpawn;
	newSpawn.angle = Vector3{ 0.0f, 180.0f, 0.0f };
	newSpawn.position = testSuper->transform()->Position();
	newSpawn.layerName = L"MainTargetEnemy";
	newSpawn.name = L"MainTarget";
	newSpawn.gameObject = testSuper->Clone();
	newSpawn.gameObject->Awake();
	AIPilot* aiPilot = static_cast<AIPilot*>(newSpawn.gameObject->GetComponent(L"AIPilot"));
	aiPilot->setDestination(player->transform()->Position() + Vector3{ 0.0f, ConvertFeetToWorld(4000.0f), ConvertFeetToWorld(10000.0f) * -5.0f });
	killTriggerDescription.spawnTargets.push_back(newSpawn);
	killTriggerDescription.standByStatus = false;
	killTriggerDescription.targetLayer = L"MainTargetEnemy";
	killTriggerDescription.triggerName = L"FirstTrigger";
	killTriggerDescription.sceneNumber = 0;

	KillSpawnTrigger* newKillSpawnTrigger = KillSpawnTrigger::Create(killTriggerDescription);
	triggerBox->AddTrigger(newKillSpawnTrigger);


	PositionSpawnTrigger::TriggerDescription positionTriggerDescription;
	newSpawn.angle = Vector3{0.0f, 180.0f, 0.0f};
	newSpawn.position = player->transform()->Position() + Vector3{ 0.0f, ConvertFeetToWorld(4000.0f), ConvertFeetToWorld(6000.0f) * 5.0f };
	newSpawn.layerName = L"MainTargetEnemy";
	newSpawn.name = L"MainTarget";
	newSpawn.gameObject = testSuper;
	newSpawn.gameObject->Awake();
	aiPilot = static_cast<AIPilot*>(testSuper->GetComponent(L"AIPilot"));
	aiPilot->setDestination(player->transform()->Position() + Vector3{ 0.0f, ConvertFeetToWorld(4000.0f), ConvertFeetToWorld(10000.0f) * -5.0f });
	positionTriggerDescription.spawnTargets.push_back(newSpawn);
	positionTriggerDescription.altitudeActive = false;
	positionTriggerDescription.position = player->transform()->Position() + Vector3{ 0.0f, 0.0f, ConvertFeetToWorld(2000.0f) * 5.0f };
	positionTriggerDescription.standByStatus = true;
	positionTriggerDescription.surveillanceTarget.push_back(player);
	positionTriggerDescription.triggerName = L"InitialTrigger";
	positionTriggerDescription.sceneNumber = 0;
	positionTriggerDescription.linkedTrigger.push_back(newKillSpawnTrigger);

	PositionSpawnTrigger* newPositionSpawnTrigger = PositionSpawnTrigger::Create(positionTriggerDescription);
	triggerBox->AddTrigger(newPositionSpawnTrigger);

	triggerBox->Awake();

	wingVaporTrailSystem->EnlistGameObject(player);
	wingVaporTrailSystem->EnlistGameObject(enemyObject);
	wingVaporTrailSystem->EnlistGameObject(testSuper);

	Sound()->LoadSound("../Bin/Resources/Sounds/BGMs/", "ms01.wav");


	uiLayer->GetGameObject(L"Rader")->Awake();
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
