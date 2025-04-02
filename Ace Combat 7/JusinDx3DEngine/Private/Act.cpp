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
	for (auto& scene : scenes)
	{
		Base::Destroy(scene);
	}
	scenes.clear();
}

Act* Act::Create(void)
{
	Act* newInstance = new Act();
	return newInstance;
}

Act* Act::Create(Scene* baseScene, Scene* loadingScene)
{
	Act* newInstance = new Act(baseScene, loadingScene);
	return newInstance;
}

void Act::Render(void)
{
	if (currentScene != nullptr)
		currentScene->Render();

	EngineInstance()->RenderManager()->Render();
}


UINT Act::AddScene(Scene* scene)
{
	scene->Start();
	scenes.push_back(scene);
	return static_cast<UINT>(scenes.size()) - 1;
}

HRESULT Act::ChangeScene(UINT sceneNumber)
{
	if (sceneNumber >= scenes.size())
		return E_FAIL;

	currentScene = scenes[sceneNumber];

	//TODO : 로딩 씬으로 전환


	return S_OK;
}

void Act::CompleteLoadingScene(void)
{
	currentScene->Release();
	currentScene = nextScene;
}
