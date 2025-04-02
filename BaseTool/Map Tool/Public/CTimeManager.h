#pragma once
#include "Base.h"
#include <thread>

class CTimeManager : public CBase
{
public:
	CTimeManager(void) {}
	virtual ~CTimeManager(void) {};
	//CTimeManager(const TimeManager&) = delete;
	//CTimeManager& operator=(const TimeManager&) = delete;

	static CTimeManager* Create(void);

	void Start(void);
	void Update(void);
	virtual void Free(void) {}
	bool IsFrameElapsed(void);

	const unsigned int& FPS(void) const { return fps; }
	const float& deltaTime(void) const { return deltaT; }

	void setTargetFPS(int value) { baseFPS = value; setDuration(); }

private:
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	TimePoint prevTime;

	int baseFPS = 60;
	unsigned int frameCount = 0;
	unsigned int fps = 0;

	float frameTime = 0.0f;
	float deltaT = 0.0f;
	float frameDuration = 0.0f;
	float frameTimer = 0.0f;

	void setDuration(void) { frameDuration = 1.0f / static_cast<float>(baseFPS); }
};