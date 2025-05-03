#include "pch.h"
#include "TU160.h"

#include "F15EHandler.h"
#include "FlightMovement.h"
#include "ModelLoader.h"
#include "MainCamera.h"
#include "FireControlSystem.h"
#include "SuperClassAIPilot.h"
#include "SuperClassAutoPilot.h"
#include "RMWR.h"
#include "StandardMissile.h"

#include "Collider.h"



TU160::TU160(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext), model(nullptr), gearModel(nullptr), flightModule(nullptr), boneHandler(nullptr), fcs(nullptr), autoPilot(nullptr), collider(nullptr)
{
}

TU160::TU160(const TU160& other) : Engine::GameObject(other)
{
    model = static_cast<Engine::Model*>(GetComponent(L"FullModel"));
    gearModel = static_cast<Engine::Model*>(GetComponent(L"GearModel"));
    flightModule = static_cast<FlightMovement*>(GetComponent(L"FlightMovement"));
    //boneHandler = static_cast<AircraftBoneHandler*>(GetComponent(L"AircraftBoneHandler"));
    fcs = static_cast<FireControlSystem*>(GetComponent(L"FCS"));
    autoPilot = static_cast<AutoPilot*>(GetComponent(L"AutoPilot"));
    collider = static_cast<Engine::Collider*>(GetComponent(L"Collider"));
}

void TU160::Free(void)
{
    Engine::GameObject::Free();
}

TU160* TU160::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
    TU160* newInstance = new TU160(dxDevice, dxDeviceContext);
    if (FAILED(newInstance->Start()))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }
    return newInstance;
}

TU160* TU160::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& metaData)
{
    TU160* newInstance = new TU160(dxDevice, dxDeviceContext);
    newInstance->metaData = metaData;
    if (FAILED(newInstance->Start()))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }
    return newInstance;
}

Engine::GameObject* TU160::Clone(void)
{
    return nullptr;
}

#define aircraftGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
#define gearGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))


HRESULT TU160::Start(void)
{
    AddCollider(collider);
    CreateTransform();

    transformComponent->Position() = Vector3(2000.0f, ConvertFeetToWorld(3000.0f), 2000.0f);
    transformComponent->Position().y /= 50.0f;
    transformComponent->SetAngle(Vector3(0.0f, 25.0f, 0.0f));

    metaData.aircraftInfomation = TU160MetaData();
    transformComponent->Scale() = Vector3::one() * 50.0f;
    transformComponent->Scale() = Vector3::one();
    Matrix matrix;
    aircraftGlobalMatrix(matrix);
    ConvertModel(metaData.aircraftInfomation.modelFilePathA, metaData.aircraftInfomation.modelFilePath, matrix);

    if (FAILED(LoadModel(metaData.aircraftInfomation.modelFilePath, model))) return E_FAIL;

    model->SetAnimation(0);
    AddComponent(model, L"FullModel");

    //boneHandler = F15EHandler::Create(dxDevice, dxDeviceContext);
    //AddComponent(boneHandler, L"AircraftBoneHandler");
    //boneHandler->Awake();

    // model setting

    flightModule = FlightMovement::Create(dxDevice, dxDeviceContext, transformComponent, metaData.aircraftInfomation.flightSpec);
    if (flightModule == nullptr) return E_FAIL;
    flightModule->SetSuper();
    AddComponent(flightModule, L"FlightMovement");
    //static_cast<AircraftBoneHandler*>(boneHandler)->LinkYoke(flightModule->yoke);

    fcs = FireControlSystem::Create(dxDevice, dxDeviceContext, metaData.aircraftInfomation);
    if (fcs == nullptr) return E_FAIL;
    fcs->SetStandardMissile(StandardMissile::Create(dxDevice, dxDeviceContext));
    fcs->SetUniqueMissile(nullptr);
    AddComponent(fcs, L"FCS");
    autoPilot = SuperClassAutoPilot::Create(dxDevice, dxDeviceContext);
    if (autoPilot == nullptr) return E_FAIL;
    AddComponent(autoPilot, L"AutoPilot");
    autoPilot->Awake();

    Engine::OBB::Description description = Engine::OBB::Description();
    //description.center = Vector3{ 50.0f, 50.0f, 50.0f } * 50.0f;
    description.center = Vector3::zero();
    description.extents = Vector3{ 1.0f, 1.0f, 1.0f };

    DirectX::XMStoreFloat4(&description.quaternion, DirectX::XMQuaternionIdentity());

    collider = Engine::Collider::Create(dxDevice, dxDeviceContext, &description);
    if (collider == nullptr) return E_FAIL;

    AddComponent(collider, L"Collider");
    AddComponent(RMWR::Create(dxDevice, dxDeviceContext), L"RMWR");

    SuperClassAIPilot* pilot = SuperClassAIPilot::Create(dxDevice, dxDeviceContext, metaData);
    AddComponent(pilot, L"AIPilot");
    pilot->Awake();

    return S_OK;
}

HRESULT TU160::Awake(void)
{
    //for (auto& component : components)
    //{
    //    component.second->Awake();
    //}

    Vector3 wayPoint = { 0.0f, ConvertFeetToWorld(10000.0f), 0.0f};
    autoPilot->SetDestination(wayPoint); 
    return S_OK;

}

void TU160::Update(void)
{
    if (!active)
        return;
    Engine::GameObject::Update();
}

void TU160::LateUpdate(void)
{
    if (!active)
        return;
    Engine::GameObject::LateUpdate();
    AddRenderObject(RenderType::NonBlend, this);
}

void TU160::FixedUpdate(void)
{
    if (!active)
        return;
    Engine::GameObject::FixedUpdate();
}

void TU160::Render(void)
{
    if (!active)
        return;
    transformComponent->Render();
    model->Render();
    collider->Render();
}