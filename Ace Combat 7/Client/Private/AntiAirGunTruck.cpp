#include "pch.h"
#include "AntiAirGunTruck.h"
#include "StaticModel.h"
#include "GroundAI.h"
#include "RMWR.h"
#include "RaderSystem.h"
#include "AntiAirFireControlSystem.h"

AntiAirGunTruck::AntiAirGunTruck(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

AntiAirGunTruck::AntiAirGunTruck(const AntiAirGunTruck& other) : Engine::GameObject(other)
{
}

void AntiAirGunTruck::Free(void)
{
	Engine::GameObject::Free();
}

AntiAirGunTruck* AntiAirGunTruck::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	AAGUN* newInstance = new AAGUN(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Engine::GameObject* AntiAirGunTruck::Clone(void)
{
	return new AntiAirGunTruck(*this);

}
#define aircraftGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
HRESULT AntiAirGunTruck::Start(void)
{
	ObjectInfomation infomation;
	infomation.allyInfo = false;
	infomation.startDirection = Vector3::zero();
	infomation.startPosition = Vector3::zero();
	infomation.gameObejctName = L"";
	infomation.aircraftInfomation = AAMetaData();

	if (FAILED(CreateTransform()))
		return E_FAIL;

	Matrix matrix;
	aircraftGlobalMatrix(matrix);
	::ConvertModel(infomation.aircraftInfomation.modelFilePathA, infomation.aircraftInfomation.modelFilePath, matrix);

	if (FAILED(::LoadStaticModel(infomation.aircraftInfomation.modelFilePath, model)))
		return E_FAIL;
	AddComponent(model, L"FullModel");

	AAFCS* fcs = AAFCS::Create(dxDevice, dxDeviceContext, infomation.aircraftInfomation);
	if (fcs == nullptr)
		return E_FAIL;
	AddComponent(fcs, L"FCS");

	RMWR* rmwr = RMWR::Create(dxDevice, dxDeviceContext);
	if (rmwr == nullptr)
		return E_FAIL;
	AddComponent(rmwr, L"RMWR");

	RaderSystem* radar = RaderSystem::Create(dxDevice, dxDeviceContext);
	if (radar == nullptr)
		return E_FAIL;
	radar->SetSearchMaxDistance(ConvertFeetToWorld(1000.0f) * 2.5f);
	AddComponent(radar, L"RaderSystem");

	GroundAI* aiPilot = GroundAI::Create(dxDevice, dxDeviceContext, infomation);
	if (aiPilot == nullptr)
		return E_FAIL;
	AddComponent(aiPilot, L"AIPilot");


	return S_OK;
}

HRESULT AntiAirGunTruck::Awake(void)
{
	for (auto& component : components)
	{
		if (FAILED(component.second->Awake()))
			return E_FAIL;
	}

	return S_OK;
}

void AntiAirGunTruck::Update(void)
{
	Engine::GameObject::Update();
}

void AntiAirGunTruck::LateUpdate(void)
{
	Engine::GameObject::LateUpdate();
}

void AntiAirGunTruck::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void AntiAirGunTruck::Render(void)
{
	transformComponent->Render();
	model->Render();
}
