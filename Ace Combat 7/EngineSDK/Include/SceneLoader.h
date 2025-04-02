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
		void CreateThread(std::function<void(void)> loadingFunction);

		bool EndLoading(void) { return loadEnd; }

	private:
		bool loadEnd = {false};
		std::thread* loadingThread = {nullptr};
	};

}
