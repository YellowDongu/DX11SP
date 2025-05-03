#include "pch.h"
#include "OceanTile.h"

OceanTile::OceanTile(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

OceanTile::OceanTile(const OceanTile& other) : Engine::GameObject(other)
{
	model = static_cast<Engine::StaticModel*>(GetComponent(L"StaticModel"));
}

void OceanTile::Free(void)
{
	Engine::GameObject::Free();
}

OceanTile* OceanTile::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	OceanTile* newInstance = new OceanTile(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* OceanTile::Clone(void)
{
	return new OceanTile(*this);
}

HRESULT OceanTile::Start(void)
{
	Matrix matrix;
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)));

	//ConvertModel("../Bin/Resources/Environment/Common/Sea/COM_Sea_100km.FBX", L"../Bin/Resources/Environment/Common/Sea/COM_Sea_100km.model", matrix);
	ConvertModel("../Bin/Resources/Environment/SouthernIsland/Terrain/SOU_OCEAN.FBX", L"../Bin/Resources/Environment/SouthernIsland/Terrain/SOU_OCEAN.model", matrix);
	if (FAILED(CreateTransform()))
		return E_FAIL;
	//if (FAILED(::LoadStaticModel(L"../Bin/Resources/Environment/Common/Sea/COM_Sea_100km.model", model)))
	if (FAILED(::LoadStaticModel(L"../Bin/Resources/Environment/SouthernIsland/Terrain/SOU_OCEAN.model", model)))
		return E_FAIL;
	AddComponent(model, L"StaticModel");
	if (FAILED(::LoadTexture(L"../Bin/Resources/Environment/Common/Sea/Textures/Sea_SeaWaves_N.dds", L"../Bin/Resources/Environment/Common/Sea/Textures/Sea_SeaWaves_N.dds", waveNormalTexture)))
		return E_FAIL;


	return S_OK;
}

void OceanTile::Update(void)
{
}

void OceanTile::LateUpdate(void)
{
	AddRenderObject(RenderType::NonBlend, this);
}

void OceanTile::FixedUpdate(void)
{
}

void OceanTile::Render(void)
{
	transformComponent->Render();
	model->Render();
}
