#include "pch.h"
#include "RaderSystem.h"


RaderSystem::RaderSystem(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::Component(dxDevice, dxDeviceContext)
{
}

RaderSystem::RaderSystem(const RaderSystem& other) : Engine::Component(other)
{
}

void RaderSystem::Free(void)
{

}

RaderSystem* RaderSystem::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	RaderSystem* newInstance = new RaderSystem(dxDevice, dxDeviceContext);
	return nullptr;
}

Engine::Component* RaderSystem::Clone(void)
{
	return nullptr;
}

HRESULT RaderSystem::Awake(void)
{
	return E_NOTIMPL;
}

void RaderSystem::Update(void)
{
	//if (allyLayer.empty())
	//	LayerSearch();
}

void RaderSystem::LateUpdate(void)
{
}

void RaderSystem::FixedUpdate(void)
{
}

void RaderSystem::SearchEnemy(void)
{
}

HRESULT RaderSystem::LayerSearch(void)
{
	return E_FAIL;
	Engine::Layer* layer = EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"");
	if (layer == nullptr)
		return E_FAIL;

	allyLayer.push_back(layer);

	layer = EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"");
	if (layer == nullptr)
		return E_FAIL;

	enemyLayer.push_back(layer);



	//unknownLayer.push_back(layer);

	return S_OK;
}