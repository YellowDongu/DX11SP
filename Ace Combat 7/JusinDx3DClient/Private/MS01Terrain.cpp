#include "pch.h"
#include "MS01Terrain.h"
#include "StaticModel.h"

MS01Terrain::MS01Terrain(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

MS01Terrain::MS01Terrain(const MS01Terrain& other) : Engine::GameObject(other)
{
}

void MS01Terrain::Free(void)
{
	Engine::GameObject::Free();
}

MS01Terrain* MS01Terrain::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	MS01Terrain* newInstance = new MS01Terrain(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

HRESULT MS01Terrain::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;
	transformComponent->Scale() = Vector3::one() * 0.01f;

	model = Engine::StaticModel::Create(dxDevice, dxDeviceContext, "D:\\PJ\\C++\\Class\\JusinDx3D\\JusinDx3DClient\\Bin\\SouthernIsland\\Terrain\\SOU_TERRAIN.fbx");
	if (model == nullptr)
		return E_FAIL;
	Engine::GameObject::AddComponent(model, L"StaticModel");


	return S_OK;
}

void MS01Terrain::Update(void)
{
	Engine::GameObject::Update();
}

void MS01Terrain::LateUpdate(void)
{
	Engine::GameObject::LateUpdate();
	AddRenderObject(RenderType::NonBlend, this);
}

void MS01Terrain::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void MS01Terrain::Render(void)
{
	transformComponent->Render();
	GetCurrentShader()->PassNumber(1);
	model->Render();
}
