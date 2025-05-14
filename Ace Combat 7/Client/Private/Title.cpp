#include "pch.h"
#include "Title.h"

#include "TitleBackground.h"
#include "MainCamera.h"
#include "DefaultModelShader.h"
#include "ScreenFadeOut.h"
#include "MainMenuParts.h"
ScreenFadeOut* fadeout = nullptr;
MainMenuParts* parts = nullptr;
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
	Background* title = Background::Create(dxDevice, dxContext);
	baseLayer->AddGameObject(L"Background", title);
	MainCamera* mainCamera = MainCamera::Create(dxDevice, dxContext);
	baseLayer->AddGameObject(L"MainCamera", mainCamera);
	fadeout = ScreenFadeOut::Create(dxDevice, dxContext);
	fadeout->SetFadeOut(1.0f);
	baseLayer->AddGameObject(L"ZZZScreenFadeOut", fadeout);
	parts = MainMenuParts::Create(dxDevice, dxContext);
	baseLayer->AddGameObject(L"MainMenuParts", parts);
	
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
	if (!loadEnd)
		loadEnd = EngineInstance()->SceneManager()->loadComplete();
	else
	{
		if (!nextScene)
		{
			if (parts->IsInMainMenu())
			{
				nextScene = Input()->getButtonDown(KeyType::Space);
				if(nextScene)
					fadeout->SetFadeOut(true);
			}
		}
		else
		{
			if (fadeout->fadeOutStatus() == 1.0f)
				EngineInstance()->SceneManager()->CompleteLoadingScene();
		}
	}
	
}

void Title::End(void)
{
	Scene::End();
}
