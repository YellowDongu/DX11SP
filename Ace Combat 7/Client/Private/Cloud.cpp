#include "pch.h"
#include "Cloud.h"

Cloud::Cloud(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

Cloud::Cloud(const Cloud& other) : Engine::GameObject(other)
{

}

void Cloud::Free(void)
{
	Engine::GameObject::Free();
}

Cloud* Cloud::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	Cloud* newInstance = new Cloud(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* Cloud::Clone(void)
{
	return new Cloud(*this);
}

HRESULT Cloud::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;



	return S_OK;
}

HRESULT Cloud::Awake(void)
{
	return E_NOTIMPL;
}

void Cloud::Update(void)
{
	Engine::GameObject::Update();
}

void Cloud::LateUpdate(void)
{
	Engine::GameObject::LateUpdate();
	AddRenderObject(RenderType::Effect, this);
}

void Cloud::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void Cloud::Render(void)
{
}
