#include "pch.h"
#include "EndText.h"

EndText::EndText(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

EndText::EndText(const EndText& other) : Engine::UIObject(other)
{
}

void EndText::Free(void)
{
	Base::DestroyInstance(text);
}

EndText* EndText::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	EndText* newInstance = new EndText(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

HRESULT EndText::Start(void)
{
	if (FAILED(::LoadFont(L"../Bin/ACES07_RegularLarge.spritefont")))
		return E_FAIL;
	text = ::CreateText(L"../Bin/ACES07_RegularLarge.spritefont");
	if (text == nullptr)
		return E_FAIL;

	Engine::GameObject::active = false;

	return S_OK;
}

HRESULT EndText::Awake(void)
{
	return S_OK;
}

void EndText::FixedUpdate(void)
{
}

void EndText::Update(void)
{
}

void EndText::LateUpdate(void)
{
	if (!Engine::GameObject::active)
		return;

	::AddRenderObject(RenderType::UI, this);
}

void EndText::Render(void)
{
	text->RenderText(L"DEMO END", Vector2::zero(), Vector2::one(), float4{1.0f,1.0f ,1.0f ,1.0f }, 0.0f, true);
}
