#include "Foundation.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"

using namespace Engine;


void Scene::Awake(void)
{
	for (auto& layer : layers)
	{
		for (auto& gameObject : layer.second->GameObjectList())
		{
			gameObject.second->Awake();
		}
	}
}

void Scene::Update(void)
{
	for (auto& layer : layers)
	{
		layer.second->Update();
	}
}

void Scene::LateUpdate(void)
{
	for (auto& layer : layers)
	{
		layer.second->LateUpdate();
	}
}

void Scene::FixedUpdate(void)
{
	for (auto& layer : layers)
	{
		layer.second->FixedUpdate();
	}
}

void Scene::Render(void)
{
}

void Scene::End(void)
{
	for (auto& layer : layers)
	{
		Base::Destroy(layer.second);
	}
	layers.clear();
}

void Scene::AddLayer(Layer* layer, const std::wstring& key)
{
	auto layerIterator = layers.find(key);
	if (layerIterator != layers.end())
		return;

	layers[key] = layer;
}

Layer* Scene::FindLayer(const std::wstring& key)
{
	auto layer = layers.find(key);
	if (layer == layers.end())
		return nullptr;
	return layer->second;
}

void Scene::Free(void)
{
	End();
}
