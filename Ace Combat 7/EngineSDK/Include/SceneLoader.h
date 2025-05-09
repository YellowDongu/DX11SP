#pragma once

#include "Base.h"

namespace Engine
{
	class Scene;
	class SceneLoader final : public Base
	{
	public:
		SceneLoader(void) = default;
		virtual ~SceneLoader(void) = default;
		virtual void Free(void) override;
		static SceneLoader* Create(void);
		void Update(void);
		void BeginLoading(Scene* scene);

		void LoadScene(Scene* scene);

		bool EndLoading(void) { return threadDone; }
		void ResetStatus(void) { threadDone = false; }

	private:
		std::atomic<bool> threadDone = false;
		std::thread* loadingThread = {nullptr};
	};

}
