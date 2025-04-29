#include "Foundation.h"
#include "TextureManager.h"

using namespace Engine;

TextureManager::TextureManager(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : device(dxDevice), context(dxDeviceContext)
{
	device->AddRef(); 
	context->AddRef();
}

TextureManager::~TextureManager(void)
{
	device->Release();
	context->Release();
}

void TextureManager::Free(void)
{
	for (auto& texture : textures)
	{
		texture.second->Release();
	}
	textures.clear();
}

TextureManager* TextureManager::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	TextureManager* newInstance = new TextureManager(dxDevice, dxDeviceContext);
	return newInstance;
}

HRESULT TextureManager::GetTexture(const std::wstring& key, ID3D11ShaderResourceView*& texture)
{
	auto textureIterator = textures.find(key);
	if (textureIterator == textures.end())
	{
		texture = nullptr;
		return E_FAIL;
	}
	texture = textureIterator->second;

	return S_OK;
}

HRESULT TextureManager::SetTexture(const std::wstring& key, const std::wstring& path)
{
	ID3D11ShaderResourceView* newTexture = nullptr;
	auto textureIterator = textures.find(key);
	if (textureIterator != textures.end())
		return E_FAIL;

	HRESULT result = DirectX::CreateDDSTextureFromFile(device, path.c_str(), nullptr, &newTexture);
	if (SUCCEEDED(result))
	{
		textures.insert({key, newTexture});
		return S_OK;
	}
		

	result = DirectX::CreateWICTextureFromFile(device, path.c_str(), nullptr, &newTexture);
	if (SUCCEEDED(result))
	{
		textures.insert({ key, newTexture });
		return S_OK;
	}

	return result;
}