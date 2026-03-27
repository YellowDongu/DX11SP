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
#include "AboveText.h"

#include "DefaultModelShader.h"

#include "MS01Terrain.h"
#include "OceanTile.h"
#include "SkyBox.h"

#include "MainCamera.h"

#include "Player.h"
#include "F15E.h"
#include "SU33.h"
#include "TU160.h"
#include "SurfaceToAirMissileTruck.h"
#include "AntiAirGunTruck.h"

#include "Trigger.h"

#include "WingVaporTrail.h"
#include "MissileTrail.h"
#include "Explosion.h"
#include "Cloud.h"
#include "ScreenFadeOut.h"
#include "EndText.h"

#include "AIPilot.h"

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
void MS01::Start(void)
{
	ID3D11Device* dxDevice = DxDevice();
	ID3D11DeviceContext* dxDeviceContext = DxDeviceContext();

	if(FAILED(EngineInstance()->ColliderSystem()->LoadNavMesh(L"../Bin/MS01.NvMsh")))
		ErrMsg(L"NAVMESH FAILED");

	shader = ::GetShader(L"DefaultModelVSShader.hlsl");
	if (shader == nullptr)
	{
		shader = Engine::DefaultModelShader::Create(dxDevice, dxDeviceContext);
		if (FAILED(::AddShader(shader->shaderFile, shader)))
			ErrMsg(L"SHADER FAILED");
	}

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


	ObjectInfomation infomation;
	infomation.immortality = true;
	infomation.allyInfo = 1;
	/*ALLY*/
	if (aircraftSelected)
	{
		player = Player::Create(dxDevice, dxDeviceContext, playerInfomation);
	}
	else
	{
		infomation.aircraftInfomation = F14DMetaData();
		player = Player::Create(dxDevice, dxDeviceContext, infomation);
	}
	player->Awake();
	player->transform()->Position() = Vector3(1643.0f, 2.0f, 405.0f);
	player->transform()->Position() = Vector3(1643.0f, ConvertFeetToWorld(1500.0f) * 2.5f, 405.0f);
	Ally->AddGameObject(L"Player", player);

	infomation.gameObejctName = L"Wingman";
	infomation.leader = player;
	infomation.wingmanPosition = Vector3::one();
	infomation.immortality = true;
	allyObject = F15E::Create(dxDevice, dxDeviceContext, infomation);
	allyObject->Awake();
	allyObject->transform()->Position() = player->transform()->Position() + Vector3{0.0f, ConvertFeetToWorld(500.0f) * 2.5f, 0.0f};
	allyObject->transform()->SetAngle(Vector3::zero());
	Ally->AddGameObject(L"WingMan", allyObject);


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
	ScreenFadeOut* fadeOut = ScreenFadeOut::Create(dxDevice, dxDeviceContext);
	fadeOut->SetFadeOutStatus(1.0f); fadeOut->SetFadeOut(false);
	uiLayer->AddGameObject(L"ZZZScreenFadeOut", fadeOut);
	uiLayer->AddGameObject(L"ZZZZEndText", EndText::Create(dxDevice, dxDeviceContext));
	AboveText* tutorialText = AboveText::Create(dxDevice, dxDeviceContext);
	uiLayer->AddGameObject(L"AboveText", tutorialText);
	tutorialText->Awake();
	

	::Sound()->LoadSound("../Bin/Resources/Sounds/BGMs/", "ms01.wav", bgm);
	SetTrigger();


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

bool bgmOn = true;
void MS01::Update(void)
{
	if (bgmOn)
	{
		if (bgmChannel == nullptr)
		{
			Sound()->Play(bgmChannel, bgm, false, 165000.0f);
		}
		else if (!Sound()->checkPlaying(bgmChannel))
			Sound()->Play(bgmChannel, bgm, false, 17530.0f);
	}
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

	F15E* f15E = nullptr;
	SU33* su33 = nullptr;
	TU160* tu160 = TU160::Create(dxDevice, dxDeviceContext);
	AAGUN* aa = AAGUN::Create(dxDevice, dxDeviceContext);
	SAM* sam = SAM::Create(dxDevice, dxDeviceContext);

	Engine::Layer* extra = Engine::Layer::Create();
	AddLayer(extra, L"ExtraLayer");
	TriggerBox* triggerBox = TriggerBox::Create(dxDevice, dxDeviceContext);
	extra->AddGameObject(L"TriggerBox", triggerBox);

	{
		MS01EndTrigger* endTrigger = MS01EndTrigger::Create();
		triggerBox->AddTrigger(endTrigger);

		killTriggerDescription.standByStatus = false;
		killTriggerDescription.targetLayer = L"MainTargetEnemy";
		killTriggerDescription.triggerName = L"MS01EndTrigger";
		killTriggerDescription.sceneNumber = 0;
		killTriggerDescription.linkedTrigger.push_back(endTrigger);

		newKillSpawnTrigger = KillSpawnTrigger::Create(killTriggerDescription);
		triggerBox->AddTrigger(newKillSpawnTrigger);
		triggerStorage.push_back(newKillSpawnTrigger);
	}

	{ // forth
		killTriggerDescription.spawnTargets.clear();
		ObjectInfomation infomation;
		infomation.allyInfo = 2;
		infomation.gameObejctName = L"TU160TGR4TGT01";
		newSpawn.layerName = L"MainTargetEnemy";
		infomation.leader = nullptr;
		infomation.wingmanPosition = Vector3::one();
		infomation.immortality = false;
		newSpawn.gameObject = TU160::Create(dxDevice, dxDeviceContext, infomation);
		newSpawn.gameObject->Awake();
		newSpawn.position = Vector3{ 210.00f,  ConvertFeetToWorld(1000.0f) * 2.5f, 710.0f };
		aiPilot = static_cast<AIPilot*>(newSpawn.gameObject->GetComponent(L"AIPilot"));
		aiPilot->setDestination(player->transform()->Position());
		newSpawn.gameObject->transform()->SetAngle((player->transform()->Position() - newSpawn.position).getDirection());
		newSpawn.angle = Vector3::zero();
		newSpawn.name = L"TU160TGR4TGT01";
		killTriggerDescription.spawnTargets.push_back(newSpawn);

		infomation.gameObejctName = L"SU33TGR04TGT02";
		infomation.leader = newSpawn.gameObject;
		su33 = SU33::Create(dxDevice, dxDeviceContext, infomation);
		newSpawn.position = Vector3{ 200.00f,  ConvertFeetToWorld(1000.0f) * 2.5f, 705.0f };
		newSpawn.angle = Vector3::zero();
		newSpawn.name = L"SU33TGR04TGT02";
		newSpawn.gameObject = su33;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);

		infomation.gameObejctName = L"SU33TGR04TGT03";
		infomation.wingmanPosition = { 2.0f, 2.0f };
		su33 = SU33::Create(dxDevice, dxDeviceContext, infomation);
		newSpawn.position = Vector3{ 205.00f,  ConvertFeetToWorld(1000.0f) * 2.5f, 700.0f };
		newSpawn.angle = Vector3::zero();
		newSpawn.name = L"SU33TGR04TGT03";
		newSpawn.gameObject = su33;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);

		infomation.gameObejctName = L"SU33TGR04TGT04";
		su33 = SU33::Create(dxDevice, dxDeviceContext, infomation);
		infomation.wingmanPosition = { -1.0f, -1.0f };
		newSpawn.position = Vector3{ 195.00f,  ConvertFeetToWorld(1000.0f) * 2.5f, 715.0f };
		newSpawn.angle = Vector3::zero();
		newSpawn.name = L"SU33TGR04TGT04";
		newSpawn.gameObject = su33;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	



		killTriggerDescription.standByStatus = false;
		killTriggerDescription.targetLayer = L"MainTargetEnemy";
		killTriggerDescription.triggerName = L"FourthTrigger";
		killTriggerDescription.sceneNumber = 0;
		killTriggerDescription.linkedTrigger.clear();
		for (auto& trigger : triggerStorage)
		{
			killTriggerDescription.linkedTrigger.push_back(trigger);
		}
		triggerStorage.clear();
	
		newKillSpawnTrigger = KillSpawnTrigger::Create(killTriggerDescription);
		triggerBox->AddTrigger(newKillSpawnTrigger);
		triggerStorage.push_back(newKillSpawnTrigger);
	}


	{ // third
//239.36 29.35 1767.89 SAM
//256.81 29.04 1772.98 AA
//242.49 29.02 1763.18 AA
//242.60 29.69 1769.40 SAM

		killTriggerDescription.spawnTargets.clear();
		newSpawn.angle = Vector3::zero();
		newSpawn.position = Vector3{ 239.36f, 29.35f, 1767.89f };
		newSpawn.layerName = L"MainTargetEnemy";
		newSpawn.name = L"SAMTGR03TGT01";
		newSpawn.gameObject = SAM::Create(dxDevice, dxDeviceContext);
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	
		newSpawn.position = Vector3{ 256.81f, 29.04f, 1772.98f };
		newSpawn.layerName = L"Enemy";
		newSpawn.name = L"AATGR03NONTGT01";
		newSpawn.gameObject = AAGUN::Create(dxDevice, dxDeviceContext);
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	
		newSpawn.position = Vector3{ 242.49f, 29.02f, 1763.18f };
		newSpawn.layerName = L"Enemy";
		newSpawn.name = L"AATGR03NONTGT02";
		newSpawn.gameObject = AAGUN::Create(dxDevice, dxDeviceContext);
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	
		newSpawn.position = Vector3{ 242.60f, 29.69f, 1769.40f };
		newSpawn.layerName = L"MainTargetEnemy";
		newSpawn.name = L"SAMTGR03TGT02";
		newSpawn.gameObject = SAM::Create(dxDevice, dxDeviceContext);
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
		
	
		ObjectInfomation infomation;
		infomation.allyInfo = 2;
		infomation.gameObejctName = L"SU33TGR01NONTGT01";
		newSpawn.layerName = L"Enemy";
		infomation.leader = nullptr;
		infomation.wingmanPosition = Vector3::one();
		infomation.immortality = false;
		su33 = SU33::Create(dxDevice, dxDeviceContext, infomation);
		newSpawn.position = Vector3{ 210.00f,  ConvertFeetToWorld(1000.0f) * 2.5f, 1610.0f };
		newSpawn.angle = Vector3::zero();
		newSpawn.name = L"SU33TGR03TGT01";
		newSpawn.gameObject = su33;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	
		infomation.gameObejctName = L"SU33TGR01NONTGT02";
		infomation.leader = su33;
		su33 = SU33::Create(dxDevice, dxDeviceContext, infomation);
		newSpawn.position = Vector3{ 200.00f,  ConvertFeetToWorld(1000.0f) * 2.5f, 1605.0f };
		newSpawn.angle = Vector3::zero();
		newSpawn.name = L"SU33TGR03TGT02";
		newSpawn.gameObject = su33;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);

		infomation.gameObejctName = L"SU33TGR01NONTGT03";
		infomation.wingmanPosition = {2.0f, 2.0f};
		su33 = SU33::Create(dxDevice, dxDeviceContext, infomation);
		newSpawn.position = Vector3{ 205.00f,  ConvertFeetToWorld(1000.0f) * 2.5f, 1600.0f };
		newSpawn.angle = Vector3::zero();
		newSpawn.name = L"SU33TGR03TGT03";
		newSpawn.gameObject = su33;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);

		infomation.gameObejctName = L"SU33TGR01NONTGT04";
		su33 = SU33::Create(dxDevice, dxDeviceContext, infomation);
		infomation.wingmanPosition = { -1.0f, -1.0f };
		newSpawn.position = Vector3{ 195.00f,  ConvertFeetToWorld(1000.0f) * 2.5f, 1615.0f };
		newSpawn.angle = Vector3::zero();
		newSpawn.name = L"SU33TGR03TGT04";
		newSpawn.gameObject = su33;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);

	
		killTriggerDescription.standByStatus = false;
		killTriggerDescription.targetLayer = L"MainTargetEnemy";
		killTriggerDescription.triggerName = L"ThirdTrigger";
		killTriggerDescription.sceneNumber = 0;
		killTriggerDescription.linkedTrigger.clear();
		for (auto& trigger : triggerStorage)
		{
			killTriggerDescription.linkedTrigger.push_back(trigger);
		}
		triggerStorage.clear();
	
		newKillSpawnTrigger = KillSpawnTrigger::Create(killTriggerDescription);
		triggerBox->AddTrigger(newKillSpawnTrigger);
		triggerStorage.push_back(newKillSpawnTrigger);
	}

	{ // second
		killTriggerDescription.spawnTargets.clear();

		newSpawn.angle = Vector3::zero();
		newSpawn.position = Vector3{ 1227.0f, 0.1f, 2218.0f };
		newSpawn.layerName = L"Enemy";
		newSpawn.name = L"SAMTGR02NONTGT01";
		newSpawn.gameObject = SAM::Create(dxDevice, dxDeviceContext);
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	
		newSpawn.position = Vector3{ 1226.0f, 0.1f, 2210.0f };
		newSpawn.name = L"AATGR02NONTGT01";
		newSpawn.gameObject = AAGUN::Create(dxDevice, dxDeviceContext);
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	
		newSpawn.position = Vector3{ 1238.0f, 0.1f, 2210.0f };
		newSpawn.name = L"SAMTGR02NONTGT02";
		newSpawn.gameObject = SAM::Create(dxDevice, dxDeviceContext);
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
		

		ObjectInfomation infomation;
		infomation.allyInfo = 2;
		infomation.gameObejctName = L"F15ETGR02NONTGT02";
		infomation.leader = nullptr;
		infomation.wingmanPosition = Vector3::one();
		infomation.immortality = false;
		f15E = F15E::Create(dxDevice, dxDeviceContext, infomation);
		newSpawn.position = Vector3{ 1238.0f,  ConvertFeetToWorld(500.0f) * 2.5f, 2210.0f };
		newSpawn.angle = Vector3::zero();
		newSpawn.name = L"F15ETGR02NONTGT02";
		newSpawn.gameObject = f15E;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	
		infomation.gameObejctName = L"F15ETGR02NONTGT01";
		infomation.leader = f15E;
		f15E = F15E::Create(dxDevice, dxDeviceContext, infomation);
		newSpawn.position = Vector3{ 1248.0f,  ConvertFeetToWorld(500.0f) * 2.5f, 2215.0f };
		newSpawn.angle = Vector3::zero();
		newSpawn.name = L"F15ETGR02NONTGT01";
		newSpawn.gameObject = f15E;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	

		newSpawn.position = Vector3{ 1084.644f, 78.3438f, 2601.74f };
		newSpawn.layerName = L"MainTargetEnemy";
		newSpawn.name = L"AATGR02TGT01";
		newSpawn.gameObject = AAGUN::Create(dxDevice, dxDeviceContext);
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	
		newSpawn.position = Vector3{ 1087.531f, 77.9125f, 2606.046f };
		newSpawn.name = L"SAMTGR02TGT01";
		newSpawn.gameObject = sam;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	
		newSpawn.position = Vector3{ 1091.271f,76.991f,2611.697f };
		newSpawn.name = L"AATGR02TGT02";
		newSpawn.gameObject = aa;
		newSpawn.gameObject->Awake();
		killTriggerDescription.spawnTargets.push_back(newSpawn);
	
		killTriggerDescription.standByStatus = false;
		killTriggerDescription.targetLayer = L"MainTargetEnemy";
		killTriggerDescription.triggerName = L"SecondTrigger";
		killTriggerDescription.sceneNumber = 0;
		killTriggerDescription.linkedTrigger.clear();
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
		positionTriggerDescription.linkedTrigger.clear();
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
	x = -500.0f;
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
