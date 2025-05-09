#include "pch.h"
#include "MS01Terrain.h"
#include "StaticModel.h"

MS01Terrain::MS01Terrain(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

MS01Terrain::MS01Terrain(const MS01Terrain& other) : Engine::GameObject(other)
{
	model = static_cast<Engine::StaticModel*>(GetComponent(L"StaticModel"));
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

Engine::GameObject* MS01Terrain::Clone(void)
{
	return new MS01Terrain(*this);
}

HRESULT MS01Terrain::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;
	transformComponent->Scale() = Vector3::one() * 0.01f;

	::ConvertModel("..\\Bin\\SouthernIsland\\Terrain\\SOU_TERRAIN.fbx", L"..\\Bin\\SouthernIsland\\Terrain\\SOU_TERRAIN.model", DirectX::XMMatrixIdentity(), true);
	
	//model = Engine::StaticModel::Create(dxDevice, dxDeviceContext, "..\\Bin\\SouthernIsland\\Terrain\\SOU_TERRAIN.fbx");
	::LoadStaticModel(L"..\\Bin\\SouthernIsland\\Terrain\\SOU_TERRAIN.model", model);
	if (model == nullptr)
		return E_FAIL;
	Engine::GameObject::AddComponent(model, L"StaticModel");

	AddComponent(model, L"StaticModel");
	if (FAILED(::LoadTexture(L"../Bin/Resources/Environment/Common/Sea/Textures/Sea_SeaWaves_N.dds", L"../Bin/Resources/Environment/Common/Sea/Textures/Sea_SeaWaves_N.dds", waveNormalTexture)))
		return E_FAIL;


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
	GetCurrentShader()->PassNumber(2);
	GetCurrentShader()->BindTexture("globalNormalTexture", waveNormalTexture);
	transformComponent->Render();
	model->Render();
	GetCurrentShader()->PassNumber(0);
}
