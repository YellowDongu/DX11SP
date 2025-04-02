#include "Foundation.h"
#include "SceneLoader.h"
#include "Scene.h"


using namespace Engine;

void SceneLoader::Update(void)
{
	if (loadingThread)
	{
		if (!loadingThread->joinable())
		{
			loadEnd = true;
			loadingThread->join();
			// delete loadingThread;
			loadingThread = nullptr;
		}

	}

}

void SceneLoader::BeginLoading(Scene* scene)
{
	CreateThread(std::bind(&Scene::Start, scene));
}

void SceneLoader::CreateThread(std::function<void(void)> loadingFunction)
{
	loadingThread = new std::thread(loadingFunction);

}
