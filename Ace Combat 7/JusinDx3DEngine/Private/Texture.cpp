#include "Foundation.h"
#include "Texture.h"

using namespace Engine;

Texture::Texture(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Component(dxDevice, dxDeviceContext)
{
}

Texture::Texture(const Texture& other) : Component(other)
{
}

void Texture::Free(void)
{



}

Texture* Texture::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	Texture* newInstance = new Texture(dxDevice, dxDeviceContext);
	return newInstance;
}

Component* Texture::Clone(void)
{
	Texture* cloneInstance = new Texture(*this);
	return cloneInstance;
}

HRESULT Texture::ChangeTexture(const std::wstring key)
{
	return E_NOTIMPL;
	return S_OK;
}

void Texture::Render(void)
{
}
