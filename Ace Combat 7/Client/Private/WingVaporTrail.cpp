#include "pch.h"
#include "WingVaporTrail.h"

WingVaporTrail::WingVaporTrail(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::Component(dxDevice, dxDeviceContext)
{
}

WingVaporTrail::WingVaporTrail(const WingVaporTrail& other) : Engine::Component(other), vertexBuffer(other.vertexBuffer), indexBuffer(other.indexBuffer)
{
	if (vertexBuffer != nullptr)
		vertexBuffer->AddRef();
	if (indexBuffer != nullptr)
		indexBuffer->AddRef();
}

void WingVaporTrail::Free(void)
{
	if (vertexBuffer != nullptr)
		vertexBuffer->Release();
	if (indexBuffer != nullptr)
		indexBuffer->Release();

}

WingVaporTrail* WingVaporTrail::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	WingVaporTrail* newInstance = new WingVaporTrail(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::Component* WingVaporTrail::Clone(void)
{
	return new WingVaporTrail(*this);
}

HRESULT WingVaporTrail::Start(void)
{
	return E_NOTIMPL;
}

HRESULT WingVaporTrail::Awake(void)
{
	return E_NOTIMPL;
}

void WingVaporTrail::Update(void)
{
}

void WingVaporTrail::LateUpdate(void)
{
}

void WingVaporTrail::FixedUpdate(void)
{
}

void WingVaporTrail::Render(void)
{
}

HRESULT WingVaporTrail::SetVertex(void)
{
	return E_NOTIMPL;
}

HRESULT WingVaporTrail::CreateBuffer(void)
{
	return E_NOTIMPL;
}
