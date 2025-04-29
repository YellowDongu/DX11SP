#include "pch.h"
#include "MS01TerrainD.h"
#include "StaticModel.h"

MS01TerrainD::MS01TerrainD(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

MS01TerrainD::MS01TerrainD(const MS01TerrainD& other) : Engine::GameObject(other)
{
	model = static_cast<Engine::StaticModel*>(GetComponent(L"StaticModel"));
}

void MS01TerrainD::Free(void)
{
	Engine::GameObject::Free();
}

MS01TerrainD* MS01TerrainD::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	MS01TerrainD* newInstance = new MS01TerrainD(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* MS01TerrainD::Clone(void)
{
	return new MS01TerrainD(*this);
}

HRESULT MS01TerrainD::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;
	transformComponent->Scale() = Vector3::one() * 0.5f;

	::ConvertModel("../Bin/Resources/Environment/SouthernIsland/Terrain/SOU_TERRAIN_D.FBX", L"../Bin/Resources/Environment/SouthernIsland/Terrain/SOU_TERRAIN_D.model", DirectX::XMMatrixIdentity(), true);
	::LoadStaticModel(L"../Bin/Resources/Environment/SouthernIsland/Terrain/SOU_TERRAIN_D.model", model);
	if (model == nullptr)
		return E_FAIL;
	Engine::GameObject::AddComponent(model, L"StaticModel");


	return S_OK;
}

void MS01TerrainD::Update(void)
{
	Engine::GameObject::Update();
}

void MS01TerrainD::LateUpdate(void)
{
	Engine::GameObject::LateUpdate();
	AddRenderObject(RenderType::NonBlend, this);
}

void MS01TerrainD::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void MS01TerrainD::Render(void)
{
	transformComponent->Render();
	model->Render();
}
