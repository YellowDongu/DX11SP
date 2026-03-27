#include "pch.h"
#include "SurfaceToAirMissileTruck.h"
#include "SurfaceToAirMissileFireControlSystem.h"
#include "RMWR.h"
#include "RaderSystem.h"
#include "GroundAI.h"

SurfaceToAirMissileTruck::SurfaceToAirMissileTruck(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

SurfaceToAirMissileTruck::SurfaceToAirMissileTruck(const SurfaceToAirMissileTruck& other) : Engine::GameObject(other)
{
	model = static_cast<Engine::StaticModel*>(GetComponent(L"FullModel"));
	collider = static_cast<Engine::Collider*>(GetComponent(L"Collider"));
	rmwr = static_cast<RadarMissileWarningReceiver*>(GetComponent(L"RMWR"));
	fcs = static_cast<FireControlSystem*>(GetComponent(L"FCS"));
}

void SurfaceToAirMissileTruck::Free(void)
{
	Engine::GameObject::Free();
}

SurfaceToAirMissileTruck* SurfaceToAirMissileTruck::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	SurfaceToAirMissileTruck* newInstance = new SurfaceToAirMissileTruck(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* SurfaceToAirMissileTruck::Clone(void)
{
	return new SAM(*this);
}

#define aircraftGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
HRESULT SurfaceToAirMissileTruck::Start(void)
{
	ObjectInfomation infomation;
	infomation.allyInfo = 2;
	infomation.startDirection = Vector3::zero();
	infomation.startPosition = Vector3::zero();
	infomation.gameObejctName = L"";
	infomation.aircraftInfomation = SAMMetaData();

	if (FAILED(CreateTransform()))
		return E_FAIL;

	Matrix matrix;
	aircraftGlobalMatrix(matrix);
	::ConvertModel(infomation.aircraftInfomation.modelFilePathA, infomation.aircraftInfomation.modelFilePath, matrix);

	if (FAILED(::LoadStaticModel(infomation.aircraftInfomation.modelFilePath, model)))
		return E_FAIL;
	AddComponent(model, L"FullModel");

	SAMFCS* fcs = SAMFCS::Create(dxDevice, dxDeviceContext, infomation);
	if (fcs == nullptr)
		return E_FAIL;
	AddComponent(fcs, L"FCS");

	rmwr = RMWR::Create(dxDevice, dxDeviceContext);
	rmwr->SetMAXHealth(20.0f);
	if (rmwr == nullptr)
		return E_FAIL;
	AddComponent(rmwr, L"RMWR");

	RaderSystem* radar = RaderSystem::Create(dxDevice, dxDeviceContext);
	if (radar == nullptr)
		return E_FAIL;
	radar->SetSearchMaxDistance(ConvertFeetToWorld(2000.0f) * 2.5f);
	radar->SetDefendArea();
	AddComponent(radar, L"RaderSystem");

	GroundAI* aiPilot = GroundAI::Create(dxDevice, dxDeviceContext, infomation);
	if (aiPilot == nullptr)
		return E_FAIL;
	AddComponent(aiPilot, L"AIPilot");

	Engine::AABB::Description description;
	description.extents = Vector3::one() * 2.0f;
	collider = Engine::Collider::Create(dxDevice, dxDeviceContext, &description);
	AddComponent(collider, L"Collider");

	return S_OK;
}

HRESULT SurfaceToAirMissileTruck::Awake(void)
{
	for (auto& component : components)
	{
		if (FAILED(component.second->Awake()))
			return E_FAIL;
	}
	return S_OK;
}

void SurfaceToAirMissileTruck::Update(void)
{
	Engine::GameObject::Update();
	AddCollider(collider);
}

void SurfaceToAirMissileTruck::LateUpdate(void)
{
	Engine::GameObject::LateUpdate();
	::AddRenderObject(RenderType::NonBlend, this);
}

void SurfaceToAirMissileTruck::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void SurfaceToAirMissileTruck::Render(void)
{
	transformComponent->Render();
	model->Render();
	collider->Render();
}
