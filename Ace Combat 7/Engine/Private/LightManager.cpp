#include "Foundation.h"
#include "LightManager.h"

using namespace Engine;

void Engine::LightManager::Free(void)
{
}

LightManager* Engine::LightManager::Create(ID3D11Device* device, ID3D11DeviceContext* context)
{
	return nullptr;
}

HRESULT Engine::LightManager::Add_Light(Light& lightStruct, const std::wstring& key)
{
	return E_NOTIMPL;
}

HRESULT Engine::LightManager::BindLight(Shader* shader, Light& light)
{
	return E_NOTIMPL;
}

HRESULT Engine::LightManager::BindWorldLight(Shader* shader)
{
	return E_NOTIMPL;
}
