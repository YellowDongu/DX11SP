#pragma once
#include "Base.h"
#include "Scene.h"
#include "SceneLoader.h"

namespace Engine
{
	class ENGINEDLL Act final : public Base
	{
	public:
		Act(void);
		Act(Scene* baseScene, Scene* loadingScene);
		virtual ~Act(void) = default;
		virtual void Free(void);
	public:
		static Act* Create(void);
		static Act* Create(Scene* baseScene, Scene* loadingScene);

		void Update(void);
		void LateUpdate(void);
		void FixedUpdate(void);
		void Render(void);

		UINT AddScene(Scene* scene);
		Scene* GetScene(UINT sceneNumber);
		void CompleteLoadingScene(void);
		void ChangeLoadingScene(UINT loadingSceneNumber);
		void ReadyOtherScene(UINT loadingSceneNumber, UINT targetSceneNumber);
		void ForceSetCurrentScene(UINT sceneNumber) { currentScene = scenes[sceneNumber]; }
		Scene* CurrentScene(void) { return currentScene; }
		Scene* LoadingScene(void) { return loadingScene; }
		bool loadComplete(void) { if (loadingScene == nullptr) return false; return loader->EndLoading(); }

	private:
		bool Loading = false, loadEnd = false;
		std::vector<Scene*> scenes;
		Scene* currentScene{nullptr};
		Scene* loadingScene{nullptr};
		SceneLoader* loader{nullptr};
	};

}