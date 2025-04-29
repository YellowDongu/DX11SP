#pragma once
#include "Base.h"
#include "Scene.h"


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

		inline void Update(void) { if(currentScene) currentScene->Update(); }
		inline void LateUpdate(void);
		inline void FixedUpdate(void) { if(currentScene) currentScene->FixedUpdate(); }
		void Render(void);



		UINT AddScene(Scene* scene);
		HRESULT ChangeScene(UINT sceneNumber);
		void CompleteLoadingScene(void);
		Scene* CurrentScene(void) { return currentScene; }

	private:
		std::vector<Scene*> scenes;
		Scene* currentScene;
		Scene* loadingScene;
		Scene* nextScene;
	};

}