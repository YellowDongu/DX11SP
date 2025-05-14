#include "pch.h"
#include "MainMenuParts.h"
#include "TitleBackground.h"
MainMenuParts::MainMenuParts(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void MainMenuParts::Free(void)
{
	Base::DestroyInstance(text);
	//GameObject::Free();
}

MainMenuParts* MainMenuParts::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	MainMenuParts* newInstance = new MainMenuParts(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* MainMenuParts::Clone(void)
{
	return nullptr;
	//BackGround* newInstance = new BackGround(*this);
	//if (FAILED(newInstance->Awake()))
	//{
	//	Base::Destroy(newInstance);
	//	return nullptr;
	//}
	//return newInstance;
}

HRESULT MainMenuParts::Start(void)
{
	text = ::CreateText(L"../Bin/ACES07.spritefont");
	if (text == nullptr)
	{
		if (FAILED(::LoadFont(L"../Bin/ACES07.spritefont")))
			return E_FAIL;

		text = ::CreateText(L"../Bin/ACES07.spritefont");
	}
	::Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "TurnToMainMenu.wav");
	::Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "MenuSelected.wav");
	::Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "MenuChange.wav");
	::Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "MenuCancel.wav");
	::Sound()->LoadSound("../Bin/Resources/Sounds/BGMs/", "main.wav");
	TurnToMainMenu = ::Sound()->FindSound(L"TurnToMainMenu.wav");
	MainMenuBGM = ::Sound()->FindSound(L"main.wav");
	MenuSelected = ::Sound()->FindSound(L"MenuSelected.wav");
	MenuChange = ::Sound()->FindSound(L"MenuChange.wav");
	MenuCancel = ::Sound()->FindSound(L"MenuCancel.wav");
	return S_OK;
}

HRESULT MainMenuParts::Awake(void)
{
	return S_OK;
}

void MainMenuParts::FixedUpdate(void)
{
}

void MainMenuParts::Update(void)
{
	if (background == nullptr)
		background = static_cast<Background*>(::EngineInstance()->SceneManager()->LoadingScene()->FindLayer(L"BaseLayer")->GetGameObject(L"Background"));

	if (InMainMenu)
	{
		bool select = Input()->getButtonDown(KeyType::Space) || Input()->getButtonDown(KeyType::Return);
		if (Input()->getButtonDown(KeyType::UP))
		{
			menuSelect--;
			Sound()->Play(effectSoundChannel, MenuChange);
			//Sound()->Play(MenuChange);
		}
		else if (Input()->getButtonDown(KeyType::DOWN))
		{
			menuSelect++;
			Sound()->Play(effectSoundChannel, MenuChange);
		}

		if (select)
		{
			Sound()->Play(effectSoundChannel, MenuSelected);
			switch (menuSelect)
			{
			case 0:
				break;
			case 1:
				PostQuitMessage(0);
				break;
			case 2:
				break;
			default:
				menuSelect = 0;
				break;
			}
		}



		if (!::Sound()->checkPlaying(bgmChannel))
			::Sound()->Play(bgmChannel, MainMenuBGM);
	}
	else
	{
		if (tempBoolean)
		{
			if (!::Sound()->checkPlaying(effectSoundChannel))
			{
				background->EndSplashScreen();
				InMainMenu = true;
				tempBoolean = false;
				::Sound()->Play(bgmChannel, MainMenuBGM);
			}
		}

		if (Input()->getButtonDown(KeyType::Space) && background->IsSplashScreenEnd())
		{
			tempBoolean = true;
			::Sound()->Play(effectSoundChannel, TurnToMainMenu);
		}
	}
}
 
void MainMenuParts::LateUpdate(void)
{
	if (!background->IsSplashScreenEnd())
		return;
	AddRenderObject(RenderType::UI, this);
}

void MainMenuParts::Render(void)
{
	if (InMainMenu)
	{
		switch (menuSelect)
		{
		case 0:
			text->RenderText(L"Start", Vector2::zero(), Vector2::one(), float4{ 1.0f,1.0f ,1.0f ,1.0f }, 0.0f);
			break;
		case 1:
			text->RenderText(L"Quit", Vector2::zero(), Vector2::one(), float4{ 1.0f,1.0f ,1.0f ,1.0f }, 0.0f);
			break;
		default:
			menuSelect = 0;
			break;
		}
	}
	else
	{
		text->RenderText(L"Press SPACE to continue", Vector2::zero(), Vector2::one(), float4{ 1.0f,1.0f ,1.0f ,1.0f }, 0.0f);
	}



}

void MainMenuParts::Main(void)
{
	// 1. start
	// 2. end
	text->RenderText(L"Start", Vector2::zero(), Vector2::one(), float4{1.0f,1.0f ,1.0f ,1.0f }, 0.0f);



}
