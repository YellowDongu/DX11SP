#include "pch.h"
#include "F15E.h"

#include "F15EHandler.h"
#include "FlightMovement.h"
#include "ModelLoader.h"
#include "MainCamera.h"
#include "FireControlSystem.h"
#include "AutoPilot.h"
#include "RMWR.h"
#include "AIPilot.h"
#include "StandardMissile.h"

#include "Collider.h"


F15E::F15E(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : GameObject(dxDevice, dxDeviceContext), model(nullptr), gearModel(nullptr), flightModule(nullptr), boneHandler(nullptr), fcs(nullptr), autoPilot(nullptr), collider(nullptr)
{
}

F15E::F15E(const F15E& other) : GameObject(other)
{
    model = static_cast<Engine::Model*>(GetComponent(L"FullModel"));
    gearModel = static_cast<Engine::Model*>(GetComponent(L"GearModel"));
    flightModule = static_cast<FlightMovement*>(GetComponent(L"FlightMovement"));
    boneHandler = static_cast<AircraftBoneHandler*>(GetComponent(L"AircraftBoneHandler"));
    fcs = static_cast<FireControlSystem*>(GetComponent(L"FCS"));
    autoPilot = static_cast<AutoPilot*>(GetComponent(L"AutoPilot"));
    collider = static_cast<Engine::Collider*>(GetComponent(L"Collider"));
}

void F15E::Free(void)
{
    GameObject::Free();
}

F15E* F15E::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	F15E* newInstance = new F15E(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* F15E::Clone(void)
{
	return new F15E(*this);
}
#define aircraftGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
#define gearGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))

HRESULT F15E::Start(void)
{
    AddCollider(collider);
    CreateTransform();

    transformComponent->Position() = Vector3(2000.0f, ConvertFeetToWorld(3000.0f), 2000.0f) * 50.0f;
    transformComponent->Position() = Vector3(2000.0f, ConvertFeetToWorld(3000.0f), 2000.0f);
    transformComponent->Position().y /= 50.0f;
    transformComponent->SetAngle(Vector3(0.0f, 25.0f, 0.0f));

    metaData.aircraftInfomation = F15EMetaData();
    transformComponent->Scale() = Vector3::one() * 50.0f;
    transformComponent->Scale() = Vector3::one();
    Matrix matrix;
    gearGlobalMatrix(matrix);
    ConvertModel(metaData.aircraftInfomation.gearModelfilePathA, metaData.aircraftInfomation.gearModelfilePath, matrix);
    aircraftGlobalMatrix(matrix);
    ConvertModel(metaData.aircraftInfomation.modelFilePathA, metaData.aircraftInfomation.modelFilePath, matrix);
    if (FAILED(LoadModel(metaData.aircraftInfomation.gearModelfilePath, gearModel))) return E_FAIL;
    if (FAILED(LoadModel(metaData.aircraftInfomation.modelFilePath, model))) return E_FAIL;

    gearModel->SetAnimation(0);
    model->SetAnimation(0);

    AddComponent(gearModel, L"GearModel");
    AddComponent(model, L"FullModel");

    boneHandler = F15EHandler::Create(dxDevice, dxDeviceContext);
    AddComponent(boneHandler, L"AircraftBoneHandler");
    boneHandler->Awake();

    // model setting

    flightModule = FlightMovement::Create(dxDevice, dxDeviceContext, transformComponent, metaData.aircraftInfomation.flightSpec);
    if (flightModule == nullptr) return E_FAIL;
    AddComponent(flightModule, L"FlightMovement");
    static_cast<AircraftBoneHandler*>(boneHandler)->LinkYoke(flightModule->yoke);

    fcs = FireControlSystem::Create(dxDevice, dxDeviceContext, metaData.aircraftInfomation);
    if (fcs == nullptr) return E_FAIL;
    fcs->SetStandardMissile(StandardMissile::Create(dxDevice, dxDeviceContext));
    fcs->SetUniqueMissile(nullptr);
    AddComponent(fcs, L"FCS");
    autoPilot = AutoPilot::Create(dxDevice, dxDeviceContext, flightModule);
    if (autoPilot == nullptr) return E_FAIL;
    AddComponent(autoPilot, L"AutoPilot");

    Engine::OBB::Description description = Engine::OBB::Description();
    //description.center = Vector3{ 50.0f, 50.0f, 50.0f } * 50.0f;
    description.center = Vector3::zero();
    description.center = metaData.aircraftInfomation.colliderCenter;
    description.extents = Vector3{ 1.0f, 1.0f, 1.0f };
    description.extents = metaData.aircraftInfomation.colliderExtent;
    DirectX::XMStoreFloat4(&description.quaternion, DirectX::XMQuaternionIdentity());

    collider = Engine::Collider::Create(dxDevice, dxDeviceContext, &description);
    if (collider == nullptr) return E_FAIL;

    AddComponent(collider, L"Collider");
    AddComponent(RMWR::Create(dxDevice, dxDeviceContext), L"RMWR");

    AIPilot* pilot = AIPilot::Create(dxDevice, dxDeviceContext, metaData);
    AddComponent(pilot, L"AIPilot");
    pilot->Awake();

	return S_OK;
}

void F15E::Update(void)
{
    if (!active)
        return;
    Engine::GameObject::Update();
}
float delay = 0.0f;
void F15E::LateUpdate(void)
{
    if (!active)
        return;
    Engine::GameObject::LateUpdate();
    AddRenderObject(RenderType::NonBlend, this);
}

void F15E::FixedUpdate(void)
{
    if (!active)
        return;
    AddRenderObject(RenderType::NonBlend, this);
    Engine::GameObject::FixedUpdate();
}

void F15E::Render(void)
{
    if (!active)
        return;
    transformComponent->Render();
    model->Render();
    gearModel->Render();
    collider->Render();
}


