#include "pch.h"
#include "Trigger.h"

TriggerBox::TriggerBox(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

TriggerBox::TriggerBox(const TriggerBox& other) : Engine::GameObject(other)
{
}

void TriggerBox::Free(void)
{
	for (auto& trigger : triggers)
	{
		Base::DestroyInstance(trigger.second);
	}
	triggers.clear();

	Engine::GameObject::Free();
}

TriggerBox* TriggerBox::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	TriggerBox* newInstance = new TriggerBox(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Engine::GameObject* TriggerBox::Clone(void)
{
	return new TriggerBox(*this);
}

HRESULT TriggerBox::Start(void)
{
	return S_OK;
}

HRESULT TriggerBox::Awake(void)
{
	for (auto& trigger : triggers)
	{
		if (FAILED(trigger.second->Awake()))
			return E_FAIL;
	}
	return S_OK;
}

void TriggerBox::Update(void)
{
	Trigger* trigger = nullptr;
	for (auto iterator = triggers.begin(); iterator != triggers.end(); )
	{
		trigger = iterator->second;
		if (trigger->Delete())
		{
			Base::DestroyInstance(trigger);
			iterator = triggers.erase(iterator);
			continue;
		}
		else if (trigger->StandBy())
		{
			trigger->Update();
		}
		iterator++;
	}
}

void TriggerBox::LateUpdate(void)
{
	for (auto& trigger : triggers)
	{
		if (!trigger.second->Delete() && trigger.second->StandBy())
			trigger.second->LateUpdate();
	}
}

void TriggerBox::FixedUpdate(void)
{
	for (auto& trigger : triggers)
	{
		if (!trigger.second->Delete() && trigger.second->StandBy())
			trigger.second->FixedUpdate();
	}
}

Trigger* TriggerBox::FindTrigger(wchar_t* triggerName)
{
	auto iterator = triggers.find(triggerName);
	if (iterator == triggers.end())
		return nullptr;
	return iterator->second;
}

Trigger* TriggerBox::FindTrigger(const std::wstring& triggerName)
{
	auto iterator = triggers.find(triggerName);
	if (iterator == triggers.end())
		return nullptr;
	return iterator->second;
}

void TriggerBox::ForceActiveTrigger(wchar_t* triggerName)
{
	Trigger* target = FindTrigger(triggerName);
	if (target != nullptr)
		target->ActiveTrigger();
}

void PositionSpawnTrigger::Free(void)
{
	for (auto& object : spawnTargets)
	{
		Base::DestroyInstance(object.gameObject);
	}
	spawnTargets.clear();
}

PositionSpawnTrigger* PositionSpawnTrigger::Create(PositionSpawnTrigger::TriggerDescription& triggerDescription)
{
	PositionSpawnTrigger* newInstance = new PositionSpawnTrigger();
	if (FAILED(newInstance->Start(triggerDescription)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

HRESULT PositionSpawnTrigger::Start(PositionSpawnTrigger::TriggerDescription& description)
{
	SetBaseInfomation(&description);

	position = description.position;
	arrivalDistance = description.arrivalDistance;
	for (auto& target : description.surveillanceTarget)
	{
		surveillanceTarget.push_back(target);
	}
	for (auto& target : description.spawnTargets)
	{
		spawnTargets.push_back(target);
	}
	altitudeActive = description.altitudeActive;
	return S_OK;
}

HRESULT PositionSpawnTrigger::Awake(void)
{
	return S_OK;
}

void PositionSpawnTrigger::Update(void)
{
}

void PositionSpawnTrigger::LateUpdate(void)
{
	FLOAT distance = 0.0f;
	Vector3 targetPostion, objectPosition;
	targetPostion = position;
	targetPostion.y = 0.0f;

	for (auto& object : surveillanceTarget)
	{
		if(altitudeActive)
			distance = (position - object->transform()->Position()).magnitude();
		else
		{
			objectPosition = object->transform()->Position();
			objectPosition.y = 0.0f;
			distance = (targetPostion - objectPosition).magnitude();
		}
		if (distance <= arrivalDistance)
		{
			ActiveTrigger();
			return;
		}
	}
}

void PositionSpawnTrigger::ActiveTrigger(void)
{
	std::wstring layerName;
	Engine::Layer* layer = nullptr;
	for (auto& object : spawnTargets)
	{
		if (object.layerName != layerName)
		{
			layer = EngineInstance()->SceneManager()->CurrentScene()->FindLayer(object.layerName);
			layerName = object.layerName;
		}

		if (layer == nullptr)
			continue;
		layer->AddGameObject(object.name, object.gameObject);
		object.gameObject->transform()->Position() = object.position;
		object.gameObject->transform()->SetAngle(object.angle);
	}
	spawnTargets.clear();
	for (auto& trigger : linkedTrigger)
	{
		trigger->SetStandBy();
	}
	deleteThis = true;
}

void Trigger::SetBaseInfomation(Trigger::TriggerDescription* description)
{
	standby = description->standByStatus;
	name = description->triggerName;
	sceneNumber = description->sceneNumber;

	for (auto& trigger : description->linkedTrigger)
	{
		linkedTrigger.push_back(trigger);
	}
}

void KillSpawnTrigger::Free(void)
{
	for (auto& object : spawnTargets)
	{
		Base::DestroyInstance(object.gameObject);
	}
	spawnTargets.clear();
}

KillSpawnTrigger* KillSpawnTrigger::Create(KillSpawnTrigger::TriggerDescription& triggerDescription)
{
	KillSpawnTrigger* newInstance = new KillSpawnTrigger();
	if (FAILED(newInstance->Start(triggerDescription)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

HRESULT KillSpawnTrigger::Start(KillSpawnTrigger::TriggerDescription& description)
{
	SetBaseInfomation(&description);

	targetLayerName = description.targetLayer;
	for (auto& target : description.spawnTargets)
	{
		spawnTargets.push_back(target);
	}

	return S_OK;
}

HRESULT KillSpawnTrigger::Awake(void)
{
	targetLayer = EngineInstance()->SceneManager()->CurrentScene()->FindLayer(targetLayerName);
	if (targetLayer == nullptr)
		return E_FAIL;


	return S_OK;
}

void KillSpawnTrigger::Update(void)
{
	if (targetLayer->GameObjectList().empty())
		ActiveTrigger();
}

void KillSpawnTrigger::LateUpdate(void)
{
}

void KillSpawnTrigger::FixedUpdate(void)
{
}

void KillSpawnTrigger::ActiveTrigger(void)
{
	std::wstring layerName;
	Engine::Layer* layer = nullptr;
	for (auto& object : spawnTargets)
	{
		if (object.layerName != layerName)
		{
			layer = EngineInstance()->SceneManager()->CurrentScene()->FindLayer(object.layerName);
			layerName = object.layerName;
		}

		if (layer == nullptr)
			continue;
		layer->AddGameObject(object.name, object.gameObject);
		object.gameObject->transform()->Position() = object.position;
		object.gameObject->transform()->SetAngle(object.angle);
	}
	spawnTargets.clear();
	for (auto& trigger : linkedTrigger)
	{
		trigger->SetStandBy();
	}
	deleteThis = true;
}
