#include "Foundation.h"
#include "TimeManager.h"

using namespace Engine;

TimeManager::TimeManager(void)
{


}

TimeManager* Engine::TimeManager::Create(void)
{
	TimeManager* newInstance = new Engine::TimeManager();
	newInstance->Start();
	return newInstance;
}

TimeManager::~TimeManager(void)
{


}

void TimeManager::Update(void)
{
	TimePoint currentTime = Clock::now();
	std::chrono::duration<FLOAT> elapsedTime = currentTime - prevTime;

	deltaT = elapsedTime.count();

	if (deltaT > 0.5f)
		deltaT = 0.5f;
	prevTime = currentTime;
	frameCount++;
	frameTime += deltaT;
	if (frameTime >= 1.0f)
	{
		fps = frameCount / static_cast<UINT>(frameTime);

		frameTime = 0.0f;
		frameCount = 0;
	}

	for (auto& timer : timers)
	{
		timer.second->Update(deltaT);
	}
}	

void TimeManager::setTargetFPS(UINT value)
{
	if (baseFPS == value)
		return;

	baseFPS = value;
	if (value == 0)
	{
		mainTimer = nullptr;
		return;
	}

	auto exsistTimer = timers.find(value);
	Timer* timer = nullptr;
	if (exsistTimer == timers.end())
	{
		timers[value] = timer = Timer::Create(value);
	}
	else
	{
		timer = (*exsistTimer).second;
	}
	mainTimer = timer;
}

void TimeManager::setFixedUpdateFPS(UINT value)
{
	if (FixedFPS == value || value == 0)
		return;
	FixedFPS = value;

	auto exsistTimer = timers.find(value);
	Timer* timer = nullptr;
	if (exsistTimer == timers.end())
	{
		timers[value] = timer = Timer::Create(value);
	}
	else
	{
		timer = (*exsistTimer).second;
	}
	fixedTimer = timer;
}

void TimeManager::Start(void)
{
	prevTime = Clock::now();
	//setTargetFPS(60);
	setFixedUpdateFPS(60);
}

void TimeManager::Free(void)
{
	for (auto& timer : timers)
	{
		Base::DestroyInstance(timer.second);
	}
	timers.clear();
}
