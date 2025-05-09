#include "pch.h"
#include "Title.h"

#include "TitleBackground.h"
#include "MainCamera.h"
#include "DefaultModelShader.h"
#include "ScreenFadeOut.h"

ScreenFadeOut* fadeout = nullptr;
void Title::Free(void)
{
	End();
}

Title* Title::Create(bool initiate)
{
	Title* newInstance = new Title();
	if(initiate)
		newInstance->Start();
	return newInstance;
}

void Title::Start(void)
{
	ID3D11Device* dxDevice = ::DxDevice();
	ID3D11DeviceContext* dxContext = ::DxDeviceContext();

	Engine::Shader* newShader = Engine::DefaultModelShader::Create(dxDevice, dxContext);
	AddShader(L"BaseShader", newShader);


	Engine::Layer* baseLayer = Engine::Layer::Create();
	AddLayer(baseLayer, L"BaseLayer");
	TitleBackground* title = TitleBackground::Create(dxDevice, dxContext);
	baseLayer->AddGameObject(L"TitleBackground", title);
	MainCamera* mainCamera = MainCamera::Create(dxDevice, dxContext);
	baseLayer->AddGameObject(L"MainCamera", mainCamera);
	fadeout = ScreenFadeOut::Create(dxDevice, dxContext);
	baseLayer->AddGameObject(L"ZZZScreenFadeOut", fadeout);
}

void Title::Awake(void)
{
	Scene::Awake();
}

void Title::Update(void)
{
	Scene::Update();
}

void Title::LateUpdate(void)
{
	Scene::LateUpdate();
}

void Title::FixedUpdate(void)
{
	Scene::FixedUpdate();
}

void Title::Render(void)
{
	SetShader(L"BaseShader");
	Scene::Render();

	if (EngineInstance()->SceneManager()->loadComplete() && Input()->getButtonDown(KeyType::Space))
	{
		fadeout->SetFadeOut(true);
	}

	if (fadeout->fadeOutStatus() == 1.0f)
		EngineInstance()->SceneManager()->CompleteLoadingScene();
}

void Title::End(void)
{
	Scene::End();
}
