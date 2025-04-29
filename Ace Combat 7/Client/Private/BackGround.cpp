#include "pch.h"
#include "BackGround.h"

BackGround::BackGround(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void BackGround::Free(void)
{
	//GameObject::Free();
}

BackGround* BackGround::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	BackGround* newInstance = new BackGround(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* BackGround::Clone(void)
{
	BackGround* newInstance = new BackGround(*this);
	if (FAILED(newInstance->Awake()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

HRESULT BackGround::Start(void)
{
	std::wstring path = L"../Bin/UI/Menu/Boot/Assets/Campaign_Loading_bg_005.dds";
	if (FAILED(LoadTexture(path, path, backGroundTexture)))
		return E_FAIL;	
	scale = { 1.0f,1.0f };
	if (FAILED(CreateVertex(vertexBuffer, indexBuffer, indexCount, scale)))
		return E_FAIL;

	return S_OK;
}

HRESULT BackGround::Awake(void)
{
	return S_OK;
}

void BackGround::FixedUpdate(void)
{
}

void BackGround::Update(void)
{
}
 
void BackGround::LateUpdate(void)
{
	AddRenderObject(RenderType::UI, this);
}

void BackGround::Render(void)
{
	SetTexture("diffuseTexture", backGroundTexture);
	GetCurrentShader()->Render(indexBuffer, vertexBuffer, sizeof(UIVertex));
	dxDeviceContext->DrawIndexed(indexCount,0,0);
}
