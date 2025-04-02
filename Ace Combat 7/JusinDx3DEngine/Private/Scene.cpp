#include "Foundation.h"
#include "Scene.h"
#include "Layer.h"

using namespace Engine;


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
}

void Scene::AddLayer(Layer* layer, const std::wstring& key)
{
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
