#include "Foundation.h"
#include "SceneLoader.h"
#include "Scene.h"


using namespace Engine;

void SceneLoader::Free(void)
{
	if(loadingThread->joinable())
		loadingThread->join();

	delete loadingThread;
	loadingThread = nullptr;
}

SceneLoader* SceneLoader::Create(void)
{
	SceneLoader* newInstnace = new SceneLoader();
	newInstnace->loadingThread = nullptr;
	return newInstnace;
}

void SceneLoader::Update(void)
{
	if (!threadDone)
		return;

	if (loadingThread != nullptr)
	{
		delete loadingThread;
		loadingThread = nullptr;
	}

}

void SceneLoader::BeginLoading(Scene* scene)
{
	if (loadingThread != nullptr)
	{
		if(loadingThread->joinable())
			loadingThread->join();

		delete loadingThread;
		loadingThread = nullptr;
	}


	loadingThread = new std::thread(&SceneLoader::LoadScene, this, scene);
}


void SceneLoader::LoadScene(Scene* scene)
{
	try
	{
		scene->Start();
		scene->Awake();

		threadDone = true;
	}
	catch (...)
	{
		ErrMsg(L"Failed to load scene");
	}
}
