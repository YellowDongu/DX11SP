#include "Foundation.h"
#include "Layer.h"
#include "GameObject.h"

using namespace Engine;

void Layer::Update(void)
{
	for (auto& gameObject : gameObjects)
	{
		gameObject.second->Update();
	}
}

void Layer::LateUpdate(void)
{
	for (auto& gameObject : gameObjects)
	{
		gameObject.second->LateUpdate();
	}
}

void Layer::FixedUpdate(void)
{
	for (auto& gameObject : gameObjects)
	{
		gameObject.second->FixedUpdate();
	}
}

void Layer::AddGameObject(const std::wstring& key, GameObject* object)
{
	gameObjects.insert({ key, object });
}

std::vector<GameObject*> Layer::GetGameObjects(const std::wstring& key)
{
	std::vector<GameObject*> list;
	auto range = gameObjects.equal_range(key);
	for (auto it = range.first; it != range.second; ++it)
	{
		list.push_back(it->second);
	}
    return list;
}

GameObject* Layer::GetGameObject(const std::wstring& key)
{
	auto result = gameObjects.find(key);
	if (result == gameObjects.end())
		return nullptr;

	return result->second;
}

void Layer::Free(void)
{
	for (auto& gameObject : gameObjects)
	{
		Base::Destroy(gameObject.second);
	}
	gameObjects.clear();
}