#include "pch.h"
#include "TitleBackground.h"

TitleBackground::TitleBackground(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void TitleBackground::Free(void)
{
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
	std::wstring path = L"../Bin/UI/Menu/Boot/Assets/Title_bg.png";
	HRESULT result = LoadTexture(path, path, backGroundTexture);
	if (FAILED(result))
		return result;

	scale = { 1.0f,1.0f };
	if (FAILED(CreateVertex(vertexBuffer, indexBuffer, indexCount, scale)))
		return E_FAIL;


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
}

void TitleBackground::LateUpdate(void)
{
}

void TitleBackground::Render(void)
{
	SetTexture("diffuseTexture", backGroundTexture);
	GetCurrentShader()->Render(indexBuffer, vertexBuffer, sizeof(UIVertex));
	dxDeviceContext->DrawIndexed(indexCount,0,0);
}
