#include "pch.h"
#include "TargetDummy.h"

TargetDummy::TargetDummy(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

TargetDummy::TargetDummy(const GameObject& other) : Engine::GameObject(other)
{
}

TargetDummy::~TargetDummy(void)
{
}

void TargetDummy::Free(void)
{
	Engine::GameObject::Free();
}

TargetDummy* TargetDummy::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Vector3 position)
{
	TargetDummy* newInstance = new TargetDummy(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	newInstance->transform()->Position() = position;
	return newInstance;
}

Engine::GameObject* TargetDummy::Clone(void)
{
	return nullptr;
}

HRESULT TargetDummy::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;

	return S_OK;
}

void TargetDummy::Update(void)
{
}

void TargetDummy::LateUpdate(void)
{
}

void TargetDummy::FixedUpdate(void)
{
}

void TargetDummy::Render(void)
{
}
