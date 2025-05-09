#include "Foundation.h"
#include "ClonePod.h"
#include "GameObject.h"
#include "Component.h"
using namespace Engine;

ClonePod* ClonePod::Create(void)
{
	ClonePod* newInstance = new ClonePod();
	return newInstance;
}

HRESULT ClonePod::SetPrefab(std::wstring& key, GameObject* gameObject)
{
	auto prefabIterator = gameObjectPrefabs.find(key);
	if (prefabIterator != gameObjectPrefabs.end())
		return E_FAIL;

	gameObjectPrefabs[key] = gameObject;
	return S_OK;
}

HRESULT ClonePod::SetPrefab(std::wstring& key, Component* component)
{
	auto prefabIterator = componentPrefabs.find(key);
	if (prefabIterator != componentPrefabs.end())
		return E_FAIL;

	componentPrefabs[key] = component;
	return S_OK;
}

GameObject* ClonePod::CloneGameObjectPrefab(std::wstring& key)
{
	auto prefabIterator = gameObjectPrefabs.find(key);
	if (prefabIterator == gameObjectPrefabs.end())
		return nullptr;

	GameObject* clone = (*prefabIterator).second->Clone();
	//if(clone->NonDestroyObject())
	//	liveObjects[key] = clone;
	//else
	//	nonDestroyObjects[key] = clone;
	return clone;
}

Component* ClonePod::CloneComponentPrefab(std::wstring& key)
{
	auto prefabIterator = componentPrefabs.find(key);
	if (prefabIterator == componentPrefabs.end())
		return nullptr;

	Component* clone = (*prefabIterator).second->Clone();
	//if(clone->NonDestroyObject())
	//	liveObjects[key] = clone;
	//else
	//	nonDestroyObjects[key] = clone;
	return clone;
}

void ClonePod::ClearPrefab(void)
{
	for (auto& prefab : gameObjectPrefabs)
	{
		Base::DestroyInstance(prefab.second);
	}
	gameObjectPrefabs.clear();

	for (auto& prefab : componentPrefabs)
	{
		Base::DestroyInstance(prefab.second);
	}
	componentPrefabs.clear();
}

void ClonePod::Free(void)
{
	ClearPrefab();
}
