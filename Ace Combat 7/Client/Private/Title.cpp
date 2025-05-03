#include "pch.h"
#include "Title.h"

#include "TitleBackground.h"

Title::Title(void)
{
}

void Title::Free(void)
{
}

Title* Title::Create(void)
{
	Title* newInstance = new Title();
	newInstance->Start();
	return newInstance;
}

void Title::Start(void)
{
	TitleBackground* title = TitleBackground::Create(DxDevice(), DxDeviceContext());
}

void Title::Awake(void)
{
}

void Title::Update(void)
{
	Scene::Update();
}

void Title::LateUpdate(void)
{
	Scene::Update();
}

void Title::FixedUpdate(void)
{
	Scene::FixedUpdate();
}

void Title::Render(void)
{
}

void Title::End(void)
{
}
