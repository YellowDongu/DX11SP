#include "pch.h"
#include "MainMenuParts.h"
#include "Background.h"
#include "ScreenFadeOut.h"
#include "SelectPreview.h"
#include "MS01.h"
MainMenuParts::MainMenuParts(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void MainMenuParts::Free(void)
{
	Base::DestroyInstance(text);
	Base::DestroyInstance(largeText);
	line.Free();
	selectedHighlight.Free();
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
	viewPort = &::Device()->ViewPortInfomation();

	text = ::CreateText(L"../Bin/ACES07KR.spritefont");
	if (text == nullptr)
	{
		if (FAILED(::LoadFont(L"../Bin/ACES07KR.spritefont")))
			return E_FAIL;

		text = ::CreateText(L"../Bin/ACES07KR.spritefont");
	}

	largeText = ::CreateText(L"../Bin/ACES07_RegularLarge.spritefont");
	if (largeText == nullptr)
	{
		if (FAILED(::LoadFont(L"../Bin/ACES07_RegularLarge.spritefont")))
			return E_FAIL;

		largeText = ::CreateText(L"../Bin/ACES07_RegularLarge.spritefont");
	}


	if(FAILED(::Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "TurnToMainMenu.wav", TurnToMainMenu))) return E_FAIL;
	if(FAILED(::Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "MenuSelected.wav", MenuSelected))) return E_FAIL;
	if(FAILED(::Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "MenuChange.wav", MenuChange))) return E_FAIL;
	if(FAILED(::Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "MenuCancel.wav", MenuCancel))) return E_FAIL;
	if(FAILED(::Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "AircraftSelected.wav", aircraftSelected))) return E_FAIL;
	if(FAILED(::Sound()->LoadSound("../Bin/Resources/Sounds/BGMs/", "main.wav", MainMenuBGM))) return E_FAIL;
	if(FAILED(::Sound()->LoadSound("../Bin/Resources/Sounds/BGMs/", "Select.wav", SelectMenuBGM))) return E_FAIL; 

	if(FAILED(selectedHighlight.LoadUITexture(L"../Bin/Resources/UI/Menu/CommonWidget/Assets/ImagePng_Highlighted.png"))) return E_FAIL;
	if(FAILED(CreateScale(selectedHighlight.texture, scale))) return E_FAIL;
	if(FAILED(CreateVertex(selectedHighlight.vertexBuffer, selectedHighlight.indexBuffer, selectedHighlight.indexCount, scale))) return E_FAIL;

	if (FAILED(line.LoadUITexture(L"../Bin/Resources/UI/Menu/LayerMenu/LayerMenuLineMeshTexture.dds"))) return E_FAIL;
	if (FAILED(CreateScale(line.texture, scale))) return E_FAIL;
	if (FAILED(CreateVertex(line.vertexBuffer, line.indexBuffer, line.indexCount, scale))) return E_FAIL;
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
	if (InMainMenu)
	{
		MainMenuUpdate();
	}
	else if (InSelectMenu)
	{
		SelectMenuUpdate();
	}
	else if (inLoading)
	{
		if (timer > 0.0f)
		{
			if (fadeout->fadeOutStatus() == 0.0f)
			{
				timer -= ::DeltaTime();
				if (timer <= 0.0f)
				{
					if (!EngineInstance()->SceneManager()->loadComplete())
						timer += ::DeltaTime();
					else
						fadeout->SetFadeOut(true);
				}
			}
		}
		else
		{
			if (fadeout->fadeOutStatus() == 1.0f)
				*nextSceneTrigger = true;
		}
	}
	else
		TitleScreenUpdate();
}

 
void MainMenuParts::LateUpdate(void)
{
	::AddRenderObject(RenderType::UI, this);
}

void MainMenuParts::Render(void)
{
	if (InMainMenu)
	{
		if (missionThumnail)
		{
			text->RenderText(L"MISSION 01", {0.0f, 10.0f}, Vector2::one(), mainColor, 0.0f, true);
			largeText->RenderText(L"Charge Assault", { 0.0f, -50.0f }, Vector2::one(), mainColor, 0.0f, true); // 60
		}
		else
			MainMenuRender();
	}
	else if (InSelectMenu)
	{
		SelectMenuRender();
		//Matrix worldMatrix;
		//FLOAT x = Device()->ViewPortInfomation().Width * -0.4f;
		//FLOAT y = Device()->ViewPortInfomation().Height * 0.35f;
		//FLOAT highlightY = 0.0f;
		//largeText->RenderText(L"SELECT AIRCRAFT", { x, y }, Vector2::one() * 0.5f, mainColor, 0.0f); // 60
		//text->RenderText(L"OWNED", { x, y - 80.0f }, Vector2::one(), mainColor, 0.0f);
		//text->RenderText(L"F-15E", { x, y - 110.0f }, Vector2::one(), mainColor, 0.0f);
		//text->RenderText(L"F-14D", { x, y - 140.0f }, Vector2::one(), mainColor, 0.0f);



		//::GetCurrentShader()->PassNumber(4);
		//DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(alterPosition, scale, 0.0f));
		//::SetMatrix(world, worldMatrix);
		//::SetTexture(diffuseTexture, selectedHighlight.texture);
		//::GetCurrentShader()->Render(selectedHighlight.indexBuffer, selectedHighlight.vertexBuffer, UIstride);
		//dxDeviceContext->DrawIndexed(selectedHighlight.indexCount, 0, 0);
		//::GetCurrentShader()->PassNumber(0);


	}
	else if (inLoading)
	{
		FLOAT x = Device()->ViewPortInfomation().Width * 0.4f;
		FLOAT y = Device()->ViewPortInfomation().Height * -0.2f;
		text->RenderLeftAlinedText(L"MISSION 01", { x, y + 35.0f }, Vector2::one(), mainColor, 0.0f);
		largeText->RenderLeftAlinedText(L"Charge Assault", { x, y }, Vector2::one(), mainColor, 0.0f); // 60
		text->RenderLeftAlinedText(L"23.May.2025  1614  Fort Grays Island  7¡Æ 58'25\"S 9¡Æ 25'50\"W  Cloud Cover: Scattered  Operation: Deer Horn", { x, y - 105.0f }, Vector2::one(), mainColor, 0.0f);
	}
	else
	{
		if (!background->IsSplashScreenEnd())
			return;
		text->RenderText(L"PRESS SPACE TO CONTINUE", Vector2{ 0.0f, viewPort->Height * -0.125f }, Vector2::one(), white, 0.0f, true);
	}
}

void MainMenuParts::MainMenuRender(void)
{

	Vector2 position = Vector2{ viewPort->Width * -0.85f, viewPort->Height * 0.8f } * 0.5f * 0.9f;
	Vector2 scale = Vector2::one() * 1.5f;
	FLOAT interval = 16.0f * 3.0f;
	//float4 color = { 169.0f / 255.0f, 1.0f, 1.0f, 1.0f }; original Color with blur
	//float4 color = { 180.0f / 255.0f, 1.0f, 1.0f, 1.0f };
	float4 color = mainColor;
	Engine::Shader* currentShader = ::GetCurrentShader();

	text->RenderText(L"MAIN MENU", position, scale, color, 0.0f);
	position.y -= viewPort->Height * 0.15f;

	currentShader->PassNumber(4);
	Matrix worldMatrix;
	Vector2 alterPosition = position;
	scale.x = 1.0f;
	alterPosition.x *= 0.75f;
	alterPosition.y -= 14.0f + interval * static_cast<FLOAT>(menuSelect);
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(alterPosition, scale, 0.0f));
	::SetMatrix(world, worldMatrix);
	::SetTexture(diffuseTexture, selectedHighlight.texture);
	currentShader->Render(selectedHighlight.indexBuffer, selectedHighlight.vertexBuffer, UIstride);
	dxDeviceContext->DrawIndexed(selectedHighlight.indexCount, 0, 0);
	currentShader->PassNumber(0);


	scale = { 1.2f, 1.2f };
	text->RenderText(L"CAMPAIN", position, scale, color, 0.0f);
	position.y -= interval;

	text->RenderText(L"OPTION", position, scale, color, 0.0f);
	position.y -= interval;

	text->RenderText(L"QUIT GAME", position, scale, color, 0.0f);

	alterPosition = position;
	
	scale.x = 1080.0f;
	scale.y = 0.5f;
	position.x -= 16.0f;
	currentShader->BindVariable("UIcolor", &color, sizeof(float4));
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 90.0f));
	::SetMatrix(world, worldMatrix);
	::SetTexture(diffuseTexture, line.texture);
	currentShader->Render(line.indexBuffer, line.vertexBuffer, UIstride);
	dxDeviceContext->DrawIndexed(line.indexCount, 0, 0);

	position.x -= 8.0f;
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 90.0f));
	::SetMatrix(world, worldMatrix);
	::SetTexture(diffuseTexture, line.texture);
	currentShader->Render(line.indexBuffer, line.vertexBuffer, UIstride);
	dxDeviceContext->DrawIndexed(line.indexCount, 0, 0);

	scale = { 1.2f, 1.2f };
	alterPosition.y = viewPort->Height * -0.25f;
	switch (menuSelect)
	{
	case 0:
		text->RenderText(L"This is a mode where you progress through the game following the story.(Demo version)", alterPosition, scale, color, 0.0f);
		break;
	case 1:
		text->RenderText(L"You can change various settings.", alterPosition, scale, color, 0.0f);
		break;
	case 2:
		text->RenderText(L"Exit the game.", alterPosition, scale, color, 0.0f);
		break;
	default:
		break;
	}
}

void MainMenuParts::SelectMenuRender(void)
{
	Matrix worldMatrix;
	Vector2 position = Vector2{ viewPort->Width * -0.85f, viewPort->Height * 0.8f } * 0.5f * 0.9f;
	Vector2 scale = Vector2::one() * 1.5f;
	FLOAT interval = 16.0f * 2.0f;
	Engine::Shader* currentShader = ::GetCurrentShader();

	text->RenderText(L"SELECT AIRCRAFT", position, scale, mainColor, 0.0f);
	position.y -= viewPort->Height * 0.075f;

	Vector2 alterPosition = position;
	currentShader->PassNumber(4);
	scale.x = 1.0f;
	alterPosition.x *= 0.75f;
	alterPosition.y -= 14.0f + interval * static_cast<FLOAT>(preview->Selected() + 1);
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(alterPosition, scale, 0.0f));
	::SetMatrix(world, worldMatrix);
	::SetTexture(diffuseTexture, selectedHighlight.texture);
	currentShader->Render(selectedHighlight.indexBuffer, selectedHighlight.vertexBuffer, UIstride);
	dxDeviceContext->DrawIndexed(selectedHighlight.indexCount, 0, 0);
	currentShader->PassNumber(0);


	scale = { 1.2f, 1.2f };
	text->RenderText(L"OWNED", position, scale, mainColor, 0.0f);
	position.y -= interval;

	text->RenderText(L"F-15E", position, scale, mainColor, 0.0f);
	position.y -= interval;
	text->RenderText(L"F-14D", position, scale, mainColor, 0.0f);
	position.y -= interval;
	text->RenderText(L"F-16C", position, scale, mainColor, 0.0f);


	alterPosition = position;

	scale.x = 1080.0f;
	scale.y = 0.5f;
	position.x -= 16.0f;
	currentShader->BindVariable("UIcolor", &mainColor, sizeof(float4));
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 90.0f));
	::SetMatrix(world, worldMatrix);
	::SetTexture(diffuseTexture, line.texture);
	currentShader->Render(line.indexBuffer, line.vertexBuffer, UIstride);
	dxDeviceContext->DrawIndexed(line.indexCount, 0, 0);

	position.x -= 8.0f;
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 90.0f));
	::SetMatrix(world, worldMatrix);
	::SetTexture(diffuseTexture, line.texture);
	currentShader->Render(line.indexBuffer, line.vertexBuffer, UIstride);
	dxDeviceContext->DrawIndexed(line.indexCount, 0, 0);
}

void MainMenuParts::MainMenuUpdate(void)
{
	if (flag)
	{
		if (fadeout->fadeOutStatus() == 1.0f)
		{
			if (timer < 0.0f)
			{
				InMainMenu = false;
				InSelectMenu = true;
				fadeout->SetFadeOut(false);
				background->SetActive(false);
				preview->SetActive(true);
				flag = false;
				missionThumnail = false;
			}
			else
			{
				fadeout->SetFadeOut(false);
				missionThumnail = true;
			}
		}
		else if (fadeout->fadeOutStatus() == 0.0f)
		{
			timer -= ::DeltaTime();
			if (timer < 0.0f)
				fadeout->SetFadeOut(true);
		}
		return;
	}

	if (!::Sound()->checkPlaying(bgmChannel))
		::Sound()->Play(bgmChannel, MainMenuBGM);

	bool select = ::Input()->getButtonDown(KeyType::Space) || ::Input()->getButtonDown(KeyType::Return);
	if (::Input()->getButtonDown(KeyType::W) || ::Input()->getButtonDown(KeyType::UP))
	{
		if (menuSelect != 0)
			menuSelect--;
		::Sound()->Play(effectSoundChannel, MenuChange);
		//Sound()->Play(MenuChange);
	}
	else if (::Input()->getButtonDown(KeyType::S) || ::Input()->getButtonDown(KeyType::DOWN))
	{
		if (menuSelect != 2)
			menuSelect++;
		::Sound()->Play(effectSoundChannel, MenuChange);
	}

	if (select)
	{
		::Sound()->Play(effectSoundChannel, MenuSelected);
		switch (menuSelect)
		{
		case 0:
			::Sound()->stopSound(bgmChannel);
			fadeout->SetFadeOut(true);
			flag = true;
			break;
		case 1:
			break;
		case 2:
			::PostQuitMessage(0);
			break;
		default:
			menuSelect = 0;
			break;
		}
	}
}

void MainMenuParts::SelectMenuUpdate(void)
{
	if (flag)
	{
		if (secondFlag)
		{
			if (::EngineInstance()->SceneManager()->loadComplete())
			{
				switch (preview->Selected())
				{
				case 0:
				{
					ObjectInfomation infomation;
					infomation.allyInfo = 0;
					infomation.gameObejctName = L"Player";
					infomation.immortality = true;
					infomation.leader = nullptr;
					infomation.aircraftInfomation = F15EMetaData();
					::EngineInstance()->SceneManager()->CurrentScene()->End();
					static_cast<MS01*>(::EngineInstance()->SceneManager()->CurrentScene())->SetPlayerInfomation(infomation);
				}
				break;
				case 1:
					break;
				case 2:
				{
					ObjectInfomation infomation;
					infomation.allyInfo = 0;
					infomation.gameObejctName = L"Player";
					infomation.immortality = true;
					infomation.leader = nullptr;
					infomation.aircraftInfomation = F16CMetaData();
					::EngineInstance()->SceneManager()->CurrentScene()->End();
					static_cast<MS01*>(::EngineInstance()->SceneManager()->CurrentScene())->SetPlayerInfomation(infomation);
				}
					break;
				default:
				{
					ObjectInfomation infomation;
					infomation.allyInfo = 0;
					infomation.gameObejctName = L"Player";
					infomation.immortality = true;
					infomation.leader = nullptr;
					infomation.aircraftInfomation = F15EMetaData();
					::EngineInstance()->SceneManager()->CurrentScene()->End();
					static_cast<MS01*>(::EngineInstance()->SceneManager()->CurrentScene())->SetPlayerInfomation(infomation);
				}
				break;
				}
				::EngineInstance()->SceneManager()->PassiveLoadScene(0);
				secondFlag = false;
			}
		}

		if (fadeout->fadeOutStatus() == 1.0f)
		{
			inLoading = true;
			InSelectMenu = false;
			timer = 3.0f;
			::Sound()->stopSound(bgmChannel);
			fadeout->SetFadeOut(false);
			preview->SetActive(false);
			background->SetActive(true);
		}
	}
	else
	{
		if (!::Sound()->checkPlaying(bgmChannel))
			::Sound()->Play(bgmChannel, SelectMenuBGM, true);

		if (::Input()->getButtonDown(KeyType::Space) || ::Input()->getButtonDown(KeyType::Return))
		{
			fadeout->SetFadeOut(true);
			flag = true;

			::Sound()->Play(aircraftSelected);
			
			if (preview->Selected() != 1)
			{
				if (::EngineInstance()->SceneManager()->loadComplete())
				{
					switch (preview->Selected())
					{
					case 0:
					{
						ObjectInfomation infomation;
						infomation.allyInfo = 0;
						infomation.gameObejctName = L"Player";
						infomation.immortality = true;
						infomation.leader = nullptr;
						infomation.aircraftInfomation = F15EMetaData();
						::EngineInstance()->SceneManager()->CurrentScene()->End();
						static_cast<MS01*>(::EngineInstance()->SceneManager()->CurrentScene())->SetPlayerInfomation(infomation);
					}
					break;
					case 1:
						break;
					case 2:
					{
						ObjectInfomation infomation;
						infomation.allyInfo = 0;
						infomation.gameObejctName = L"Player";
						infomation.immortality = true;
						infomation.leader = nullptr;
						infomation.aircraftInfomation = F16CMetaData();
						::EngineInstance()->SceneManager()->CurrentScene()->End();
						static_cast<MS01*>(::EngineInstance()->SceneManager()->CurrentScene())->SetPlayerInfomation(infomation);
					}
					break;
					default:
					{
						ObjectInfomation infomation;
						infomation.allyInfo = 0;
						infomation.gameObejctName = L"Player";
						infomation.immortality = true;
						infomation.leader = nullptr;
						infomation.aircraftInfomation = F15EMetaData();
						::EngineInstance()->SceneManager()->CurrentScene()->End();
						static_cast<MS01*>(::EngineInstance()->SceneManager()->CurrentScene())->SetPlayerInfomation(infomation);
					}
					break;
					}
					::EngineInstance()->SceneManager()->PassiveLoadScene(0);
					secondFlag = false;
				}
				else
					secondFlag = true;
			}

		}
	}
}

void MainMenuParts::TitleScreenUpdate(void)
{
	if (background == nullptr)
		background = static_cast<Background*>(::EngineInstance()->SceneManager()->LoadingScene()->FindLayer(L"BaseLayer")->GetGameObject(L"Background"));

	if (flag)
	{
		if (!::Sound()->checkPlaying(effectSoundChannel))
		{
			background->EndSplashScreen();
			InMainMenu = true;
			flag = false;
			::Sound()->Play(bgmChannel, MainMenuBGM);
		}
	}
	else if (Input()->getButtonDown(KeyType::Space) && background->IsSplashScreenEnd())
	{
		if (fadeout == nullptr)
			fadeout = static_cast<ScreenFadeOut*>(::EngineInstance()->SceneManager()->LoadingScene()->FindLayer(L"BaseLayer")->GetGameObject(L"ZZZScreenFadeOut"));
		if (preview == nullptr)
			preview = static_cast<SelectPreview*>(::EngineInstance()->SceneManager()->LoadingScene()->FindLayer(L"BaseLayer")->GetGameObject(L"SelectPreview"));
		flag = true;
		::Sound()->Play(effectSoundChannel, TurnToMainMenu);
	}
}
