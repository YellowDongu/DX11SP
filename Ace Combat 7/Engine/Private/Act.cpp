#include "Foundation.h"
#include "Act.h"
#include "GameInstance.h"
using namespace Engine;

Act::Act(void) : currentScene(nullptr)
{
}

Act::Act(Scene* _baseScene, Scene* _loadingScene) : currentScene(_baseScene), loadingScene(_loadingScene)
{
	scenes.push_back(loadingScene);
	scenes.push_back(currentScene);
}

void Act::Free(void)
{
	Base::DestroyInstance(loader);
	for (auto& scene : scenes)
	{
		Base::Destroy(scene);
	}
	scenes.clear();
}

Act* Act::Create(void)
{
	Act* newInstance = new Act();
	newInstance->loader = SceneLoader::Create();
	return newInstance;
}

Act* Act::Create(Scene* baseScene, Scene* loadingScene)
{
	Act* newInstance = new Act(baseScene, loadingScene);
	newInstance->loader = SceneLoader::Create();
	return newInstance;
}

void Act::Update(void)
{
	if (Loading)
	{
		if (loadingScene)
			loadingScene->Update();
	}
	else
	{
		if (currentScene)
			currentScene->Update();
	}
}


void Act::LateUpdate(void)
{
	EngineInstance()->ColliderSystem()->Update();
	EngineInstance()->ColliderSystem()->LateUpdate();
	if (Loading)
	{

		if (loadingScene)
			loadingScene->LateUpdate();
	}
	else
	{
		if (currentScene == nullptr)
			return;

		currentScene->LateUpdate();
	}
}
void Act::FixedUpdate(void)
{
	if (Loading)
	{
		if (loadingScene)
			loadingScene->FixedUpdate();
	}
	else
	{
		if (currentScene)
			currentScene->FixedUpdate();
	}
}
void Act::Render(void)
{
	if (Loading)
	{
		if (loadingScene)
			loadingScene->Render();
	}
	else
	{
		if (currentScene != nullptr)
			currentScene->Render();
	}

	EngineInstance()->RenderManager()->Render();
}

UINT Act::AddScene(Scene* scene)
{
	scenes.push_back(scene);
	return static_cast<UINT>(scenes.size()) - 1;
}

Scene* Act::GetScene(UINT sceneNumber)
{
	if (sceneNumber >= scenes.size())
		return nullptr;

	return scenes[sceneNumber];
}

void Act::CompleteLoadingScene(void)
{
	if (!loader->EndLoading())
		return;

	loadingScene->End();
	loadingScene = nullptr;
	Loading = false;

	loader->ResetStatus();
	EngineInstance()->RenderManager()->SkipRender();
}

void Engine::Act::ChangeLoadingScene(UINT loadingSceneNumber)
{
	if (loadingSceneNumber >= scenes.size())
		return;

	loadingScene = scenes[loadingSceneNumber];
}

void Act::ReadyOtherScene(UINT loadingSceneNumber, UINT targetSceneNumber)
{
	if (loadingSceneNumber >= scenes.size() || targetSceneNumber >= scenes.size())
			return;

	currentScene = scenes[targetSceneNumber];
	loadingScene = scenes[loadingSceneNumber];
	Loading = true;

	loader->BeginLoading(currentScene);
}

void Engine::Act::PassiveLoadScene(UINT sceneIndex)
{
	loader->BeginLoading(scenes[sceneIndex]);

	Loading = true;
	loadEnd = false;
	loader->ResetStatus();
}
