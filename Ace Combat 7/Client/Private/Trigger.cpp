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
