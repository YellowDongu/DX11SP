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
#include "AntiAirGunTruck.h"

#include "WingVaporTrail.h"
#include "MissileTrail.h"

#include "Trigger.h"
#include "AIPilot.h"

#include "Explosion.h"
#include "Cloud.h"
Player* player = nullptr;
F15E* enemyObject = nullptr;
F15E* allyObject = nullptr;
TU160* tu160 = nullptr;
SAM* samTest = nullptr;
AAGUN* aaTest = nullptr;
MainCamera* mainCamera = nullptr;
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


	Engine::Layer* MainTargetEnemyGround = nullptr;
	Engine::Layer* MainTargetEnemy = Engine::Layer::Create();
	AddLayer(MainTargetEnemy, L"MainTargetEnemy");
	Engine::Layer* Enemy = Engine::Layer::Create();
	AddLayer(Enemy, L"Enemy");
	//Engine::Layer* GroundEnemy = nullptr;
	//Engine::Layer* NonTargetEnemy = nullptr;
	//Engine::Layer* NonTargetGroundEnemy = nullptr;
	//Engine::Layer* MainTargetAlly = nullptr;
	//Engine::Layer* MainTargetAllyGround = nullptr;
	Engine::Layer* Ally = Engine::Layer::Create();
	AddLayer(Ally, L"Ally");
	//Engine::Layer* GroundAlly = nullptr;
	//Engine::Layer* NonTargetAlly = nullptr;
	//Engine::Layer* NonTargetGroundAlly = nullptr;
	//Engine::Layer* UnKnown = nullptr;
	//Engine::Layer* GroundUnKnown = nullptr;


	/*ALLY*/
	player = Player::Create(dxDevice, dxDeviceContext);
	player->Awake();
	player->transform()->Position() = Vector3(1643.0f, 2.0f, 405.0f);
	player->transform()->Position() = Vector3(1643.0f, ConvertFeetToWorld(1500.0f) * 2.5f, 405.0f);
	Ally->AddGameObject(L"Player", player);


	ObjectInfomation infomation;
	infomation.allyInfo = 1;
	infomation.gameObejctName = L"Wingman";
	infomation.leader = player;
	infomation.wingmanPosition = Vector3::one();
	infomation.immortality = true;
	allyObject = F15E::Create(dxDevice, dxDeviceContext, infomation);
	allyObject->Awake();
	allyObject->transform()->Position() = player->transform()->Position() + Vector3{0.0f, ConvertFeetToWorld(500.0f) * 2.5f, 0.0f};
	allyObject->transform()->SetAngle(Vector3::zero());
	Ally->AddGameObject(L"WingMan", allyObject);


	enemyObject = F15E::Create(dxDevice, dxDeviceContext);
	enemyObject->Awake();
	Enemy->AddGameObject(L"MainTargetEnemy", enemyObject);

	samTest = SAM::Create(dxDevice, dxDeviceContext);
	samTest->Awake();
	samTest->transform()->Position() = player->transform()->Position() + Vector3{ 0.0f, 0.0f , 300.0f };

	aaTest = AAGUN::Create(dxDevice, dxDeviceContext);
	aaTest->Awake();
	aaTest->transform()->Position() = player->transform()->Position() + Vector3{0.0f, 0.0f , 200.0f};
	aaTest->transform()->Position().y = 1.0f;

	Engine::NavMesh* navMesh = EngineInstance()->ColliderSystem()->CurrentNavMesh();
	INT triangleIndex = navMesh->GetTriangleIndex(samTest->transform()->Position());
	if (triangleIndex != -1)
	{
		FLOAT y = navMesh->GetNode(triangleIndex)->getHeight(samTest->transform()->Position());
		samTest->transform()->Position().y = y;
	}
	triangleIndex = navMesh->GetTriangleIndex(aaTest->transform()->Position());
	if (triangleIndex != -1)
	{
		FLOAT y = navMesh->GetNode(triangleIndex)->getHeight(aaTest->transform()->Position());
		aaTest->transform()->Position().y = y;
	}
	Enemy->AddGameObject(L"SAM", samTest);
	Enemy->AddGameObject(L"AA", aaTest);


	//UIShader* uiShader = UIShader::Create(dxDevice, dxDeviceContext);
	//AddShader(L"DefaultUIShader.hlsl", uiShader);
	//Engine::DefaultUIShader* newShader = Engine::DefaultUIShader::Create(dxDevice, dxDeviceContext);
	//AddShader(newShader->shaderFile, newShader);

	Engine::Layer* enviormentLayer = Engine::Layer::Create();
	AddLayer(enviormentLayer, L"TestLayer");
	mainCamera = MainCamera::Create(dxDevice, dxDeviceContext);
	enviormentLayer->AddGameObject(L"MainCamera", mainCamera);
	player->LinkCamera(mainCamera);
	SetTerrain(dxDevice, dxDeviceContext, enviormentLayer);

	Cloud* newCloud = Cloud::Create(dxDevice, dxDeviceContext, player->transform()->Position() + Vector3{ 0.0f, 100.0f, 500.0f }, Vector3{100.0f, 25.0f, 100.0f});
	newCloud->Awake();
	particleLayer->AddGameObject(L"Cloud01", newCloud);



	Engine::Layer* uiLayer = Engine::Layer::Create();
	AddLayer(uiLayer, L"UILayer");
	uiLayer->AddGameObject(L"AttitudeIndicator", AttitudeIndicator::Create(dxDevice, dxDeviceContext, player->transform()));
	uiLayer->AddGameObject(L"FlightPathMarker", FlightPathMarker::Create(dxDevice, dxDeviceContext, player));
	uiLayer->AddGameObject(L"Rader", RaderUI::Create(dxDevice, dxDeviceContext, player, nullptr, 0));
	uiLayer->AddGameObject(L"SpeedIndicator", SpeedIndicator::Create(dxDevice, dxDeviceContext, player));
	uiLayer->AddGameObject(L"AltitudeIndicator", AltitudeIndicator::Create(dxDevice, dxDeviceContext, player));
	uiLayer->AddGameObject(L"Compass", Compass::Create(dxDevice, dxDeviceContext, player));
	uiLayer->AddGameObject(L"PlayerStatusHUD", PlayerStatusHeadUpDisplay::Create(dxDevice, dxDeviceContext, player));
	uiLayer->AddGameObject(L"IFFHUD", IFFHUD::Create(dxDevice, dxDeviceContext, this, player, mainCamera));
	uiLayer->AddGameObject(L"FCSHUD", FCSHUD::Create(dxDevice, dxDeviceContext, player, mainCamera));


	Sound()->LoadSound("../Bin/Resources/Sounds/BGMs/", "ms01.wav");
	SetTrigger();


	//testSuper = TU160::Create(dxDevice, dxDeviceContext);
	//testSuper->transform()->Position() = Vector3{ 5000.0f, ConvertFeetToWorld(10000.0f), 5000.0f };
	////enemy->AddGameObject(L"MainTarget", testSuper);
	////bgm = Sound()->FindSound(L"ms01.wav");
	////Sound()->Play(bgmChannel, bgm, 160000.0f);
	//
	//
	//Engine::Layer* extra = Engine::Layer::Create();
	//AddLayer(extra, L"ExtraLayer");
	//TriggerBox* triggerBox = TriggerBox::Create(dxDevice, dxDeviceContext);
	//extra->AddGameObject(L"TriggerBox", triggerBox);
	//
	//
	//KillSpawnTrigger::TriggerDescription killTriggerDescription;
	//GameObjectSpawnInfomation newSpawn;
	//newSpawn.angle = Vector3{ 0.0f, 180.0f, 0.0f };
	//newSpawn.position = testSuper->transform()->Position();
	//newSpawn.layerName = L"MainTargetEnemy";
	//newSpawn.name = L"MainTarget";
	//newSpawn.gameObject = testSuper->Clone();
	//newSpawn.gameObject->Awake();
	//AIPilot* aiPilot = static_cast<AIPilot*>(newSpawn.gameObject->GetComponent(L"AIPilot"));
	//aiPilot->setDestination(player->transform()->Position() + Vector3{ 0.0f, ConvertFeetToWorld(4000.0f), ConvertFeetToWorld(10000.0f) * -5.0f });
	//killTriggerDescription.spawnTargets.push_back(newSpawn);
	//killTriggerDescription.standByStatus = false;
	//killTriggerDescription.targetLayer = L"MainTargetEnemy";
	//killTriggerDescription.triggerName = L"FirstTrigger";
	//killTriggerDescription.sceneNumber = 0;
	//
	//KillSpawnTrigger* newKillSpawnTrigger = KillSpawnTrigger::Create(killTriggerDescription);
	//triggerBox->AddTrigger(newKillSpawnTrigger);
	//
	//
	//PositionSpawnTrigger::TriggerDescription positionTriggerDescription;
	//newSpawn.angle = Vector3{0.0f, 180.0f, 0.0f};
	//newSpawn.position = player->transform()->Position() + Vector3{ 0.0f, ConvertFeetToWorld(4000.0f), ConvertFeetToWorld(6000.0f) * 5.0f };
	//newSpawn.layerName = L"MainTargetEnemy";
	//newSpawn.name = L"MainTarget";
	//newSpawn.gameObject = testSuper;
	//newSpawn.gameObject->Awake();
	//aiPilot = static_cast<AIPilot*>(testSuper->GetComponent(L"AIPilot"));
	//aiPilot->setDestination(player->transform()->Position() + Vector3{ 0.0f, ConvertFeetToWorld(4000.0f), ConvertFeetToWorld(10000.0f) * -5.0f });
	//positionTriggerDescription.spawnTargets.push_back(newSpawn);
	//positionTriggerDescription.altitudeActive = false;
	//positionTriggerDescription.position = player->transform()->Position() + Vector3{ 0.0f, 0.0f, ConvertFeetToWorld(2000.0f) * 5.0f };
	//positionTriggerDescription.standByStatus = true;
	//positionTriggerDescription.surveillanceTarget.push_back(player);
	//positionTriggerDescription.triggerName = L"InitialTrigger";
	//positionTriggerDescription.sceneNumber = 0;
	//positionTriggerDescription.linkedTrigger.push_back(newKillSpawnTrigger);
	//
	//PositionSpawnTrigger* newPositionSpawnTrigger = PositionSpawnTrigger::Create(positionTriggerDescription);
	//triggerBox->AddTrigger(newPositionSpawnTrigger);
	//
	//triggerBox->Awake();




	for (auto& gameObject : uiLayer->GameObjectList())
	{
		gameObject.second->Awake();
	}
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

HRESULT MS01::SetTrigger(void)
{
	ID3D11Device* dxDevice = DxDevice();
	ID3D11DeviceContext* dxDeviceContext = DxDeviceContext();
	KillSpawnTrigger* newKillSpawnTrigger = nullptr;
	KillSpawnTrigger::TriggerDescription killTriggerDescription;
	GameObjectSpawnInfomation newSpawn;
	AIPilot* aiPilot = nullptr;
	std::list<Trigger*> triggerStorage;

	tu160 = TU160::Create(dxDevice, dxDeviceContext);
	AAGUN* aa = AAGUN::Create(dxDevice, dxDeviceContext);
	SAM* sam = SAM::Create(dxDevice, dxDeviceContext);
	//enemy->AddGameObject(L"MainTarget", testSuper);
	//bgm = Sound()->FindSound(L"ms01.wav");
	//Sound()->Play(bgmChannel, bgm, 160000.0f);


	Engine::Layer* extra = Engine::Layer::Create();
	AddLayer(extra, L"ExtraLayer");
	TriggerBox* triggerBox = TriggerBox::Create(dxDevice, dxDeviceContext);
	extra->AddGameObject(L"TriggerBox", triggerBox);

	{ // second
		newSpawn.angle = Vector3::zero();
		newSpawn.position = Vector3{ 1227.0f, 0.1f, 2218.0f };
		newSpawn.layerName = L"Enemy";
		newSpawn.name = L"SAMTGR01NONTGT01";
		newSpawn.gameObject = sam->Clone();
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);

		newSpawn.position = Vector3{ 1226.0f, 0.1f, 2210.0f };
		newSpawn.name = L"AATGR01NONTGT01";
		newSpawn.gameObject = aa->Clone();
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);

		newSpawn.position = Vector3{ 1238.0f, 0.1f, 2210.0f };
		newSpawn.name = L"SAMTGR01NONTGT02";
		newSpawn.gameObject = sam->Clone();
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
		
		newSpawn.position = Vector3{ 1084.644f, 78.3438f, 2601.74f };
		newSpawn.layerName = L"MainTargetEnemy";
		newSpawn.name = L"AATGR01TGT01";
		newSpawn.gameObject = aa->Clone();
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);

		newSpawn.position = Vector3{ 1087.531f, 77.9125f, 2606.046f };
		newSpawn.name = L"SAMTGR01TGT01";
		newSpawn.gameObject = sam;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);

		newSpawn.position = Vector3{ 1091.271f,76.991f,2611.697f };
		newSpawn.name = L"AATGR01TGT02";
		newSpawn.gameObject = aa;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);

		killTriggerDescription.standByStatus = false;
		killTriggerDescription.targetLayer = L"MainTargetEnemy";
		killTriggerDescription.triggerName = L"SecondTrigger";
		killTriggerDescription.sceneNumber = 0;
		for (auto& trigger : triggerStorage)
		{
			killTriggerDescription.linkedTrigger.push_back(trigger);
		}
		triggerStorage.clear();

		newKillSpawnTrigger = KillSpawnTrigger::Create(killTriggerDescription);
		triggerBox->AddTrigger(newKillSpawnTrigger);
		triggerStorage.push_back(newKillSpawnTrigger);
	}

	{// first
		PositionSpawnTrigger::TriggerDescription positionTriggerDescription;
		newSpawn.angle = Vector3{ 0.0f, 180.0f, 0.0f };
		newSpawn.position = player->transform()->Position() + Vector3{ 0.0f, ConvertFeetToWorld(4000.0f), ConvertFeetToWorld(6000.0f) * 5.0f };
		newSpawn.layerName = L"MainTargetEnemy";
		newSpawn.name = L"ACTGR00TGT01";
		newSpawn.gameObject = tu160;
		newSpawn.gameObject->Awake();
		aiPilot = static_cast<AIPilot*>(tu160->GetComponent(L"AIPilot"));
		aiPilot->setDestination(player->transform()->Position() + Vector3{ 0.0f, ConvertFeetToWorld(4000.0f), ConvertFeetToWorld(10000.0f) * -5.0f });
		positionTriggerDescription.spawnTargets.push_back(newSpawn);

		positionTriggerDescription.altitudeActive = false;
		positionTriggerDescription.standByStatus = true;
		positionTriggerDescription.position = player->transform()->Position() + Vector3{ 0.0f, 0.0f, ConvertFeetToWorld(2000.0f) * 5.0f };
		positionTriggerDescription.surveillanceTarget.push_back(player);
		positionTriggerDescription.triggerName = L"InitialTrigger";
		positionTriggerDescription.sceneNumber = 0;
		for (auto& trigger : triggerStorage)
		{
			positionTriggerDescription.linkedTrigger.push_back(trigger);
		}
		triggerStorage.clear();

		PositionSpawnTrigger* newPositionSpawnTrigger = PositionSpawnTrigger::Create(positionTriggerDescription);
		triggerBox->AddTrigger(newPositionSpawnTrigger);
	}
	triggerBox->Awake();
	return S_OK;
}

HRESULT MS01::SetTerrain(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Layer* enviormentLayer)
{
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

	FLOAT z = 0.0f, x = 2000.0f;
	for (FLOAT i = 0.0f; i < 6.0f; i++)
	{
		z = 500.0f * i;

		oceanTileClone = oceanTile->Clone();
		oceanTileClone->transform()->Position() = Vector3(x, 0.0f, z);
		enviormentLayer->AddGameObject(L"OceanTile", oceanTileClone);
	}
	enviormentLayer->AddGameObject(L"SkyBox", SkyBox::Create(dxDevice, dxDeviceContext, mainCamera));
	return S_OK;
}
