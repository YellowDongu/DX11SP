#include "pch.h"
#include "GunReticle.h"

GunReticle::GunReticle(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void GunReticle::Free(void)
{
}

GunReticle* GunReticle::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	GunReticle* newInstance = new GunReticle(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

#define innerErrorCheck(result, errmsg)					   \
if (FAILED(result))										   \
{														   \
	ErrMsg((std::wstring(L"GunReticle::Failed : ") + errmsg).c_str()); \
	return E_FAIL;										   \
}														   \


HRESULT GunReticle::Start(void)
{
	Vector2 Scale = {0.0f, 0.0f};
	innerErrorCheck(innerCircle.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_gunReticle.png"), L"Load texture - hud_gunReticle.png");
	innerErrorCheck(CreateVertex(innerCircle.vertexBuffer, innerCircle.indexBuffer, innerCircle.indexCount, scale), L"Vertex/Index Create");

	Scale = { 0.0f, 0.0f };
	innerErrorCheck(outerCircle.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_gunReticle_inner.png"), L"Load texture - hud_gunReticle_inner.png");
	innerErrorCheck(CreateVertex(outerCircle.vertexBuffer, outerCircle.indexBuffer, outerCircle.indexCount, scale), L"Vertex/Index Create");

	return S_OK;
}

HRESULT GunReticle::Awake(void)
{
	return E_NOTIMPL;
}
#undef innerErrorCheck

void GunReticle::FixedUpdate(void)
{
}

void GunReticle::Update(void)
{
}

void GunReticle::LateUpdate(void)
{
}

void GunReticle::Render(void)
{
}
