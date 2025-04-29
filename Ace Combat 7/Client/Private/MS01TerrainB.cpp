#include "pch.h"
#include "MS01TerrainB.h"
#include "StaticModel.h"

MS01TerrainB::MS01TerrainB(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

MS01TerrainB::MS01TerrainB(const MS01TerrainB& other) : Engine::GameObject(other)
{
	model = static_cast<Engine::StaticModel*>(GetComponent(L"StaticModel"));
}

void MS01TerrainB::Free(void)
{
	Engine::GameObject::Free();
}

MS01TerrainB* MS01TerrainB::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	MS01TerrainB* newInstance = new MS01TerrainB(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* MS01TerrainB::Clone(void)
{
	return new MS01TerrainB(*this);
}

HRESULT MS01TerrainB::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;
	transformComponent->Scale() = Vector3::one() * 0.5f;

	::LoadStaticModel(L"../Bin/Resources/Environment/SouthernIsland/Terrain/SOU_TERRAIN_B.model", model);
	if (model == nullptr)
		return E_FAIL;
	Engine::GameObject::AddComponent(model, L"StaticModel");


	return S_OK;
}

void MS01TerrainB::Update(void)
{
	Engine::GameObject::Update();
}

void MS01TerrainB::LateUpdate(void)
{
	Engine::GameObject::LateUpdate();
	AddRenderObject(RenderType::NonBlend, this);
}

void MS01TerrainB::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void MS01TerrainB::Render(void)
{
	transformComponent->Render();
	model->Render();
}
