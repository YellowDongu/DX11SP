#include "pch.h"
#include "MainApp.h"

#include "MS01.h"
#include "Title.h"
#include "DefaultModelShader.h"
int windowSizeX = 1920;
int windowSizeY = 1080;

MainApp::MainApp(void)
{
}

void MainApp::Start(void)
{
	Engine::DeviceInfo deviceInfo;
	RECT rect;

	ZeroMemory(&deviceInfo, sizeof(Engine::DeviceInfo));
	GetClientRect(hWnd, &rect);

	deviceInfo.hWnd = hWnd;
	deviceInfo.hInstance = hInstance;
	deviceInfo.isWindowed = true;
	
	deviceInfo.viewpoirtLength = windowSizeY;
	deviceInfo.viewpoirtWidth = windowSizeX;

	Engine::GameInstance* gameInstance = Engine::GameInstance::Create(deviceInfo);
	gameInstance->EntireInitialize();
	gameInstance->Time()->setTargetFPS(144);
	//gameInstance->Device()->SetBackBufferColor({153.0f / 256.0f * 0.8f, 217.0f / 256.0f * 0.6f, 234.0f / 256.0f * 0.8f, 1.0f});
	gameInstance->Device()->SetBackBufferColor({0.0f, 0.0f, 0.0f, 1.0f});
	auto device = DxDevice();
	auto context = DxDeviceContext();

	::EngineInstance()->CreateConsole();

	sceneManager = EngineInstance()->SceneManager();
	sceneManager->AddScene(MS01::Create());
	sceneManager->AddScene(Title::Create(true));
	sceneManager->ReadyOtherScene(1, 0);
}

HRESULT MainApp::MainLoop(void)
{
	EngineInstance()->TimeUpdate();
	std::cout << DeltaTime() << "	:	" << Time()->FPS() << std::endl;
	if (Time()->MainTimer() != nullptr)
	{
		if (!Time()->MainTimer()->AtTime())
			return E_FAIL;
	}
	EngineInstance()->InTimeUpdate();

	if (Time()->FixedTimer() != nullptr)
	{
		if (Time()->FixedTimer()->AtTime())
			FixedUpdate();
	}
	Update();
	LateUpdate();

	Device()->Render(std::bind(&MainApp::Render, this));
	return S_OK;
}

void MainApp::Update(void)
{
	sceneManager->Update();
}

void MainApp::LateUpdate(void)
{
	sceneManager->LateUpdate();
}

void MainApp::FixedUpdate(void)
{
	sceneManager->FixedUpdate();
}

void MainApp::Render(void)
{
	sceneManager->Render();

}

MainApp* MainApp::Create(void)
{
	MainApp* newInstance = new MainApp();
	newInstance->Start();
	return newInstance;
}

void MainApp::Free(void)
{
	DestroyGameInstance();
}
