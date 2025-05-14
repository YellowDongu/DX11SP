#include "pch.h"
#include "RaderSystem.h"
#include "FireControlSystem.h"
#include "SuperClassAIPilot.h"
#include "PlayerPilot.h"
#include "RMWR.h"

RaderSystem::RaderSystem(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::Component(dxDevice, dxDeviceContext)
{
}

RaderSystem::RaderSystem(const RaderSystem& other) : Engine::Component(other)
{
}

void RaderSystem::Free(void)
{

}

RaderSystem* RaderSystem::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	RaderSystem* newInstance = new RaderSystem(dxDevice, dxDeviceContext);
	return newInstance;
}

Engine::Component* RaderSystem::Clone(void)
{
	return new RaderSystem(*this);
}

HRESULT RaderSystem::Awake(void)
{
	FCS* fcs = static_cast<FCS*>(gameObject->GetComponent(L"FCS"));
	if (fcs == nullptr)
		return E_FAIL;

	currentEnemy = &fcs->LinkTarget();
	currentEmemyRMWR = &fcs->LinkTargetRMWR();


	AIPilot* pilot = static_cast<AIPilot*>(gameObject->GetComponent(L"AIPilot"));
	if (pilot == nullptr)
	{
		pilot = static_cast<AIPilot*>(gameObject->GetComponent(L"SuperClassAIPilot"));
		player = (pilot == nullptr);
	}

	if (player)
	{
		PlayerPilot* playerPilot = static_cast<PlayerPilot*>(gameObject->GetComponent(L"PlayerPilot"));
		if (playerPilot == nullptr)
			return E_FAIL;
		playerPilot->LinkObjectInfomation();
		allyFaction = true;
	}
	else
	{
		allyFaction = pilot->LinkObjectInfomation().allyInfo != 2;
		//maxDistance = FLT_MAX;
	}

	CollectLayer();
	return S_OK;
}

void RaderSystem::Update(void)
{
	if (*currentEnemy != nullptr && (*currentEnemy)->Destroy())
	{
		UnTargeting();
		for (auto iterator = sortedObjects.begin(); iterator != sortedObjects.end();)
		{
			if (iterator->second == *currentEnemy)
			{
				iterator = sortedObjects.erase(iterator);
				timer = 0.0f;
				break;
			}
			else
			{
				iterator++;
			}
		}

	}
	else
	{
		if (player)
		{
			PlayerControl();
		}
		else
		{
			SearchEnemy();
		}
	}
}

void RaderSystem::LateUpdate(void)
{
	if (*currentEnemy != nullptr && (*currentEnemy)->Destroy())
	{
		UnTargeting();
		for (auto iterator = sortedObjects.begin(); iterator != sortedObjects.end();)
		{
			if (iterator->second == *currentEnemy)
			{
				iterator = sortedObjects.erase(iterator);
				break;
			}
			else
			{
				iterator++;
			}
		}

	}

}

void RaderSystem::FixedUpdate(void)
{
}

void RaderSystem::SearchEnemy(void)
{
	timer -= DeltaTime();
	if (*currentEnemy != nullptr)
	{
		float distance = (gameObject->transform()->Position() - (*currentEnemy)->transform()->Position()).magnitude();

		if (distance >= maxDistance)
		{
			if (FAILED(LayerSearch()))
				return;

			if (sortedObjects.empty())
			{
				UnTargeting();
			}
			else
			{
				*currentEnemy = sortedObjects.front().second;
				*currentEmemyRMWR = static_cast<RMWR*>((*currentEnemy)->GetComponent(L"RMWR"));
				currentTargetIndex = 0;
			}
		}
	}
	else
	{
		if (FAILED(LayerSearch()))
			return;

		if (sortedObjects.empty())
		{
			UnTargeting();
		}
		else
		{
			*currentEnemy = sortedObjects.front().second;
			*currentEmemyRMWR = static_cast<RMWR*>((*currentEnemy)->GetComponent(L"RMWR"));
			currentTargetIndex = 0;
		}
		timer = maxTimer;
	}
}

HRESULT RaderSystem::LayerSearch(void)
{
	if (timer > 0.0f)
		return E_FAIL;

	FLOAT distance = 0.0f, angle = 0.0f;
	sortedObjects.clear();
	const Vector3& position = gameObject->transform()->Position();
	const Vector3& forward = gameObject->transform()->Forward();
	xmVector forwardDX = DirectX::XMLoadFloat3(&gameObject->transform()->Forward());
	Vector3 direction;

	if (player)
	{
		for (auto& layer : enemyLayer)
		{
			for (auto& enemyGameObject : layer->GameObjectList())
			{
				direction = enemyGameObject.second->transform()->Position() - position;
				distance = direction.magnitude();

				if (distance > maxDistance)
					continue;

				angle = DirectX::XMVectorGetX(DirectX::XMVector3Dot(forwardDX, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction))));

				sortedObjects.push_back({ angle, enemyGameObject.second });

			}
		}
		sortedObjects.sort([](const std::pair<FLOAT, Engine::GameObject*>& object, const std::pair<FLOAT, Engine::GameObject*>& subject) { return object.first > subject.first; });
	}
	else
	{
		for (auto& layer : enemyLayer)
		{
			for (auto& enemyGameObject : layer->GameObjectList())
			{
				direction = enemyGameObject.second->transform()->Position() - position;
				distance = direction.magnitude();
				if(!defend || distance < maxDistance)
					sortedObjects.push_back({ distance, enemyGameObject.second });
			}
		}
		sortedObjects.sort([](const std::pair<FLOAT, Engine::GameObject*>& object, const std::pair<FLOAT, Engine::GameObject*>& subject) { return object.first < subject.first; });

	}

	currentTargetIndex = -1;

	return S_OK;
}

void RaderSystem::CollectLayer(void)
{
	std::list<Engine::Layer*> layersBucket;
	Engine::Scene* currentScene = EngineInstance()->SceneManager()->CurrentScene();
	Engine::Layer* layer = nullptr;


	layer = currentScene->FindLayer(L"MainTargetAlly");
	if (layer != nullptr)
		layersBucket.push_back(layer);
	layer = currentScene->FindLayer(L"MainTargetAllyGround");
	if (layer != nullptr)
		layersBucket.push_back(layer);
	layer = currentScene->FindLayer(L"Ally");
	if (layer != nullptr)
		layersBucket.push_back(layer);
	layer = currentScene->FindLayer(L"GroundAlly");
	if (layer != nullptr)
		layersBucket.push_back(layer);
	layer = currentScene->FindLayer(L"NonTargetAlly");
	if (layer != nullptr)
		layersBucket.push_back(layer);
	layer = currentScene->FindLayer(L"NonTargetGroundAlly");
	if (layer != nullptr)
		layersBucket.push_back(layer);


	if (allyFaction)
	{
		allyLayer.reserve(layersBucket.size());
		for (auto& layer : layersBucket)
		{
			allyLayer.push_back(layer);
		}
	}
	else
	{
		enemyLayer.reserve(layersBucket.size());
		for (auto& layer : layersBucket)
		{
			enemyLayer.push_back(layer);
		}

	}
	layersBucket.clear();

	layer = currentScene->FindLayer(L"MainTargetEnemy");
	if (layer != nullptr)
		layersBucket.push_back(layer);
	layer = currentScene->FindLayer(L"MainTargetEnemyGround");
	if (layer != nullptr)
		layersBucket.push_back(layer);
	layer = currentScene->FindLayer(L"Enemy");
	if (layer != nullptr)
		layersBucket.push_back(layer);
	layer = currentScene->FindLayer(L"GroundEnemy");
	if (layer != nullptr)
		layersBucket.push_back(layer);
	layer = currentScene->FindLayer(L"NonTargetEnemy");
	if (layer != nullptr)
		layersBucket.push_back(layer);
	layer = currentScene->FindLayer(L"NonTargetGroundEnemy");
	if (layer != nullptr)
		layersBucket.push_back(layer);

	if (allyFaction)
	{
		enemyLayer.reserve(layersBucket.size());
		for (auto& layer : layersBucket)
		{
			enemyLayer.push_back(layer);
		}
	}
	else
	{
		allyLayer.reserve(layersBucket.size());
		for (auto& layer : layersBucket)
		{
			allyLayer.push_back(layer);
		}
	}

	return;
	layer = currentScene->FindLayer(L"UnKnown");
	layer = currentScene->FindLayer(L"GroundUnKnown");

}

void RaderSystem::PlayerControl(void)
{
	if (Input()->getButtonDown(KeyType::T))
	{
		LayerSearch();
		if (sortedObjects.empty())
		{
			currentTargetIndex = -1;
			*currentEnemy = nullptr;
			timer = 0.2f;
		}
		else
		{
			currentTargetIndex++;

			auto iterator = sortedObjects.begin();

			for (INT i = 0; i < currentTargetIndex; i++)
			{
				iterator++;
				if (iterator == sortedObjects.end())
				{
					currentTargetIndex = 0;
					iterator = sortedObjects.begin();
					break;
				}
			}
			*currentEnemy = iterator->second;
			timer = maxTimer;
		}

	}
	else
	{
		timer -= DeltaTime();
	}
}
