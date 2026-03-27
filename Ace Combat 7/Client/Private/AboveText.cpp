#include "pch.h"
#include "AboveText.h"
#include "ScreenFadeOut.h"

AboveText::AboveText(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void AboveText::Free(void)
{
	Base::DestroyInstance(text);
}

AboveText* AboveText::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	AboveText* newInstance = new AboveText(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

HRESULT AboveText::Start(void)
{
	text = ::CreateText(L"../Bin/ACES07.spritefont");
	if (text == nullptr)
		return E_FAIL;

	position = { 0.0f, ::Device()->ViewPortInfomation().Height * 0.35f };
	Engine::GameObject::active = true;

	return S_OK;
}

HRESULT AboveText::Awake(void)
{
	layer = ::EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"UILayer");
	if (layer == nullptr)
		return E_FAIL;

	fadeOutScreen = static_cast<ScreenFadeOut*>(layer->GetGameObject(L"ZZZScreenFadeOut"));
	if (fadeOutScreen == nullptr)
		return E_FAIL;

	layer = ::EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"MainTargetEnemy");
	if (layer == nullptr)
		return E_FAIL;

	return S_OK;
}

void AboveText::Update(void)
{
}

void AboveText::LateUpdate(void)
{
	if (!Engine::GameObject::active)
		return;
	::AddRenderObject(RenderType::UI, this);
	if (!incomeBattle)
	{
		if (!layer->GameObjectList().empty())
			incomeBattle = true;
	}
}

void AboveText::FixedUpdate(void)
{
}

void AboveText::Render(void)
{
	static float4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
	static Vector2 defaultScale = Vector2::one();

	if (fadeOutScreen->fadeOutStatus() != 0.0f)
		return;

	if (initial)
	{
		timer -= 0.01f;
		if (timer < 0.0f)
			initial = false;
	}
	else if (secondTimer > 0.0f)
	{
		text->RenderText(L"W to accelerate, S to decelerate", position, defaultScale, color, 0.0f, true);
		secondTimer -= ::DeltaTime();

		if (secondTimer <= 0.0f)
			timer = 2.5f;
	}
	else if (thridTimer > 0.0f)
	{
		if (timer > 0.0f)
		{
			timer -= ::DeltaTime();
		}
		else
		{
			text->RenderText(L"A/D, <v^> (Arrow keys) to rotate", position, defaultScale, color, 0.0f, true);
			text->RenderText(L"V to change view", { position.x, position.y - 16.0f * 1.5f }, defaultScale, color, 0.0f, true);
			thridTimer -= ::DeltaTime();

			if (thridTimer <= 0.0f)
				timer = 2.5f;
		}
	}
	else if (forthTimer > 0.0f)
	{
		if (timer > 0.0f)
		{
			timer -= ::DeltaTime();
		}
		else
		{
			text->RenderText(L"R to change Rader, E to change Missile", position, defaultScale, color, 0.0f, true);
			forthTimer -= ::DeltaTime();

			if (forthTimer <= 0.0f)
				timer = 2.5f;
		}
	}
	else if (incomeBattle)
	{
		if (timer > 0.0f)
		{
			text->RenderText(L"T to change target, SPACE to shoot missile, CTRL to shoot gun", position, defaultScale, color, 0.0f, true);
			timer -= ::DeltaTime();
		}
		else
		{
			if (timer <= -4.0f)
			{
				Engine::GameObject::active = true;
			}
			else if (timer <= -1.0f)
			{
				text->RenderText(L"Destroy all target", position, defaultScale, color, 0.0f, true);
			}
			timer -= ::DeltaTime();
		}
	}
}
