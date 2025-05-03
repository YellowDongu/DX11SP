#pragma once

#include "Base.h"

namespace Engine
{
	class Scene;
	class SceneLoader final : public Base
	{
	public:
		SceneLoader(void) = default;
		~SceneLoader(void) = default;

		void Update(void);
		void BeginLoading(Scene* scene);

		void LoadScene(Scene* scene);

		bool EndLoading(void) { return threadDone; }

	private:
		std::atomic<bool> threadDone = false;
		std::thread* loadingThread = {nullptr};
	};

}
