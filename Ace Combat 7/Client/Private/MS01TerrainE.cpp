#include "pch.h"
#include "MS01TerrainE.h"
#include "StaticModel.h"

MS01TerrainE::MS01TerrainE(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

MS01TerrainE::MS01TerrainE(const MS01TerrainE& other) : Engine::GameObject(other)
{
	model = static_cast<Engine::StaticModel*>(GetComponent(L"StaticModel"));
}

void MS01TerrainE::Free(void)
{
	Engine::GameObject::Free();
}

MS01TerrainE* MS01TerrainE::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	MS01TerrainE* newInstance = new MS01TerrainE(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* MS01TerrainE::Clone(void)
{
	return new MS01TerrainE(*this);
}

HRESULT MS01TerrainE::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;
	transformComponent->Scale() = Vector3::one() * 0.5f;

	::LoadStaticModel(L"../Bin/Resources/Environment/SouthernIsland/Terrain/SOU_TERRAIN_E.model", model);
	if (model == nullptr)
		return E_FAIL;
	Engine::GameObject::AddComponent(model, L"StaticModel");


	return S_OK;
}

void MS01TerrainE::Update(void)
{
	Engine::GameObject::Update();
}

void MS01TerrainE::LateUpdate(void)
{
	Engine::GameObject::LateUpdate();
	AddRenderObject(RenderType::NonBlend, this);
}

void MS01TerrainE::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void MS01TerrainE::Render(void)
{
	transformComponent->Render();
	model->Render();
}
