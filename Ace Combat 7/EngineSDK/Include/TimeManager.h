#pragma once
#include "Timer.h"

namespace Engine
{
	class ENGINEDLL TimeManager final : public Base
	{
		TimeManager(void);
		virtual ~TimeManager(void);
		virtual void Free(void) override;
	public:
		static TimeManager* Create(void);


		void Start(void);
		void Update(void);

		const UINT& FPS(void) const { return fps; }
		const FLOAT& deltaTime(void) const { return deltaT; }

		void setTargetFPS(UINT value);
		void setFixedUpdateFPS(UINT value);

		Timer* MainTimer(void) { return mainTimer; }
		Timer* FixedTimer(void) { return fixedTimer; }

	private:

		using Clock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::time_point<Clock>;

		std::map<INT, Timer*> timers;
		Timer* fixedTimer = nullptr;
		Timer* mainTimer = nullptr;

		TimePoint prevTime;

		UINT baseFPS = 0;
		UINT FixedFPS = 0;
		UINT frameCount = 0;
		UINT fps = 0;
		FLOAT frameTime = 0.0f;
		FLOAT deltaT = 0.0f;
	};

}
