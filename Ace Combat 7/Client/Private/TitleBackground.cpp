#include "pch.h"
#include "TitleBackground.h"

TitleBackground::TitleBackground(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

TitleBackground::TitleBackground(const TitleBackground& other) : Engine::UIObject(other)
{
}

void TitleBackground::Free(void)
{
	Engine::GameObject::Free();
}

TitleBackground* TitleBackground::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	TitleBackground* newInstance = new TitleBackground(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Engine::GameObject* TitleBackground::Clone(void)
{
	TitleBackground* newInstance = new TitleBackground(*this);

	if (FAILED(newInstance->Awake()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

HRESULT TitleBackground::Start(void)
{
	HRESULT result = CreateTransform();
	if (FAILED(result))
		return result;


	std::wstring path = L"../Bin/UI/Menu/Boot/Assets/Title_bg.png";
	result = LoadTexture(path, path, backGroundTexture);
	if (FAILED(result))
		return result;

	rectangle = Engine::RectanglePolygon::Create(dxDevice, dxDeviceContext);
	if (rectangle == nullptr)
		return E_FAIL;

	AddComponent(rectangle, L"RectanglePolygon");

	return S_OK;
}

HRESULT TitleBackground::Awake(void)
{
	return S_OK;
}

void TitleBackground::FixedUpdate(void)
{
}

void TitleBackground::Update(void)
{
	const D3D11_VIEWPORT& viewPortSetting = Device()->ViewPortInfomation();
	transformComponent->Scale() = Vector3{ viewPortSetting.Width, viewPortSetting.Height, 1.0f };

}

void TitleBackground::LateUpdate(void)
{
	AddRenderObject(RenderType::UI, this);
}

void TitleBackground::Render(void)
{
	transformComponent->Render();
	rectangle->Render();
}
