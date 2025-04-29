#include "pch.h"
#include "MS01TerrainA.h"
#include "StaticModel.h"

MS01TerrainA::MS01TerrainA(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

MS01TerrainA::MS01TerrainA(const MS01TerrainA& other) : Engine::GameObject(other)
{
	model = static_cast<Engine::StaticModel*>(GetComponent(L"StaticModel"));
}

void MS01TerrainA::Free(void)
{
	Engine::GameObject::Free();
}

MS01TerrainA* MS01TerrainA::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	MS01TerrainA* newInstance = new MS01TerrainA(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* MS01TerrainA::Clone(void)
{
	return new MS01TerrainA(*this);
}

HRESULT MS01TerrainA::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;
	transformComponent->Scale() = Vector3::one() * 0.5f;

	::LoadStaticModel(L"../Bin/Resources/Environment/SouthernIsland/Terrain/SOU_TERRAIN_A.model", model);
	if (model == nullptr)
		return E_FAIL;
	Engine::GameObject::AddComponent(model, L"StaticModel");


	return S_OK;
}

void MS01TerrainA::Update(void)
{
	Engine::GameObject::Update();
}

void MS01TerrainA::LateUpdate(void)
{
	Engine::GameObject::LateUpdate();
	AddRenderObject(RenderType::NonBlend, this);
}

void MS01TerrainA::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void MS01TerrainA::Render(void)
{
	transformComponent->Render();
	model->Render();
}
