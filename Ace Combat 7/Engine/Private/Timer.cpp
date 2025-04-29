#include "Foundation.h"
#include "Timer.h"

using namespace Engine;

Timer::Timer(void) : atTime(false), frame(60), timer(0.0f), frameTime(0.0f)
{
}

Timer::~Timer(void)
{
}

void Timer::Free(void)
{
}

void Timer::Start(void)
{
	SetTimer(frame);
}

void Timer::Update(FLOAT deltaTime)
{
	timer += deltaTime;
	atTime = timer >= frameTime;
	if (atTime)
	{
		timer -= frameTime;
	}
}

void Timer::SetTimer(INT FPS)
{
	atTime = false;
	frame = FPS;
	timer = 0;
	frameTime = 1.0f / static_cast<FLOAT>(FPS);
}

Timer* Timer::Create(INT fps)
{
	Timer* newInstance = new Timer();
	newInstance->SetTimer(fps);
	return newInstance;
}
