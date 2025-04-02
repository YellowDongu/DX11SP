#pragma once
#include "Base.h"

namespace Engine
{
	class ENGINEDLL Timer final : public Base
	{
	private:
		Timer(void);
	public:
		virtual ~Timer(void);
		static Timer* Create(INT fps);


		void Start(void);
		void Update(FLOAT deltaTime);

		void SetTimer(INT FPS);

		const FLOAT& deltaTime(void) const { return frameTime; }
		bool AtTime(void) { return atTime; }

	private:
		bool atTime;
		INT frame;
		FLOAT timer;
		FLOAT frameTime;
	};
}