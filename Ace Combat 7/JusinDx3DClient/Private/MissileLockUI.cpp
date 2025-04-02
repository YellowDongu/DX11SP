#include "pch.h"
#include "MissileLockUI.h"

MissileLockUI::MissileLockUI(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void MissileLockUI::Free(void)
{
}

MissileLockUI* MissileLockUI::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	MissileLockUI* newInstance = new MissileLockUI(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

HRESULT MissileLockUI::Start(void)
{



	return E_NOTIMPL;
}

HRESULT MissileLockUI::Awake(void)
{
	return E_NOTIMPL;
}

void MissileLockUI::FixedUpdate(void)
{
}

void MissileLockUI::Update(void)
{
}

void MissileLockUI::LateUpdate(void)
{
}

void MissileLockUI::Render(void)
{
}
