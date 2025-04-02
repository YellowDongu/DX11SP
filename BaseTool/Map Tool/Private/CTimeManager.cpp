#include "pch.h"
#include "CTimeManager.h"


CTimeManager* CTimeManager::Create(void)
{
	CTimeManager* newTime = new CTimeManager();
	newTime->Start();
	return newTime;
}

void CTimeManager::Start(void)
{
	prevTime = Clock::now();
	baseFPS = 60;
	setDuration();
}

void CTimeManager::Update(void)
{
	TimePoint currentTime = Clock::now();
	std::chrono::duration<float> elapsedTime = currentTime - prevTime;

	deltaT = elapsedTime.count();
	prevTime = currentTime;
	frameCount++;
	frameTime += deltaT;
	if (frameTime >= 1.0f)
	{
		fps = static_cast<unsigned int>(frameCount / frameTime);

		frameTime = 0.0f;
		frameCount = 0;
	}

	frameTimer += deltaT;

	if (frameTimer >= 1.0f)
		frameTimer = 0.0f;
}

bool CTimeManager::IsFrameElapsed(void)
{
	if (frameTimer >= frameDuration)
	{
		frameTimer -= frameDuration;
		return true;
	}
	return false;
}
