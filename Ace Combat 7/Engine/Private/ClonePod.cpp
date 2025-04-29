#include "Foundation.h"
#include "ClonePod.h"
#include "GameObject.h"
using namespace Engine;

ClonePod* ClonePod::Create(void)
{
	ClonePod* newInstance = new ClonePod();
	return newInstance;
}

HRESULT ClonePod::SetPrefab(std::wstring& key, GameObject* gameObject)
{
	auto prefabIterator = prefabs.find(key);
	if (prefabIterator != prefabs.end())
		return E_FAIL;

	prefabs[key] = gameObject;
	return S_OK;
}

GameObject* ClonePod::ClonePrefab(std::wstring& key)
{
	auto prefabIterator = prefabs.find(key);
	if (prefabIterator != prefabs.end())
		return nullptr;

	GameObject* clone = (*prefabIterator).second->Clone();
	//if(clone->NonDestroyObject())
	//	liveObjects[key] = clone;
	//else
	//	nonDestroyObjects[key] = clone;
	return clone;
}

void ClonePod::ClearPrefab(void)
{
	for (auto& prefab : prefabs)
	{
		Base::Destroy(prefab.second);
	}
	prefabs.clear();
}

void Engine::ClonePod::ClearAliveObject(void)
{
	for (auto& object : liveObjects)
	{
		Base::Destroy(object.second);
	}
	liveObjects.clear();

}

void ClonePod::Free(void)
{
	ClearPrefab();
}
