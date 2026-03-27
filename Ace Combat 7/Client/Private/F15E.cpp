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
#include "WingVaporTrail.h"
#include "RaderSystem.h"

#include "Collider.h"

AIPilot* pilot = nullptr;
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
    newInstance->metaData.allyInfo = 2;
    newInstance->metaData.gameObejctName = L"Test";
    newInstance->metaData.aircraftInfomation = F15EMetaData();
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

F15E* F15E::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& infomation)
{
    F15E* newInstance = new F15E(dxDevice, dxDeviceContext);
    newInstance->metaData = infomation;
    newInstance->metaData.aircraftInfomation = F15EMetaData();
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
#define gearGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
RaderSystem* radar;
HRESULT F15E::Start(void)
{
    if (FAILED(CreateTransform()))
        return E_FAIL;

    //transformComponent->Position() = Vector3(2000.0f, ConvertFeetToWorld(3000.0f), 2000.0f) * 50.0f;
    transformComponent->Position() = Vector3(2000.0f, ConvertFeetToWorld(3000.0f), 2000.0f);
    transformComponent->SetAngle(Vector3(0.0f, 25.0f, 0.0f));
    //transformComponent->Scale() = Vector3::one() * 50.0f;
    //transformComponent->Scale() = Vector3::one();

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

    flightModule = FlightMovement::Create(dxDevice, dxDeviceContext, metaData.aircraftInfomation.flightSpec);
    if (flightModule == nullptr) return E_FAIL;
    AddComponent(flightModule, L"FlightMovement");

    fcs = FireControlSystem::Create(dxDevice, dxDeviceContext, metaData);
    if (fcs == nullptr) return E_FAIL;
    fcs->SetStandardMissile(StandardMissile::Create(dxDevice, dxDeviceContext));
    fcs->SetUniqueMissile(nullptr);
    AddComponent(fcs, L"FCS");
    autoPilot = AutoPilot::Create(dxDevice, dxDeviceContext);
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
    RMWR* rmwr = RMWR::Create(dxDevice, dxDeviceContext);
    rmwr->SetMAXHealth(40.0f);
    AddComponent(rmwr, L"RMWR");

    pilot = AIPilot::Create(dxDevice, dxDeviceContext, metaData);
    AddComponent(pilot, L"AIPilot");

    RaderSystem* radar = RaderSystem::Create(dxDevice, dxDeviceContext);
    AddComponent(radar, L"RaderSystem");
	return S_OK;
}

HRESULT F15E::Awake(void)
{
    static_cast<AircraftBoneHandler*>(boneHandler)->LinkYoke(flightModule->yoke);
    for (auto& component : components)
    {
        if (FAILED(component.second->Awake()))
            return E_FAIL;
    }

    //if(FAILED(boneHandler->Awake()))
    //    return E_FAIL;
    //if(FAILED(autoPilot->Awake()))
    //    return E_FAIL;
    //if (FAILED(pilot->Awake()))
    //    return E_FAIL;
    //radar->Awake();

    Engine::Layer* layer = ::EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"ParticleLayer");
    if (layer == nullptr)
        return E_FAIL;

    WingVaporTrail* wingVaporParticle = static_cast<WingVaporTrail*>(layer->GetGameObject(L"WingVaporTrail"));
    if (wingVaporParticle == nullptr)
        return E_FAIL;

    wingVaporParticle->EnlistGameObject(this);


    return S_OK;
}

void F15E::Update(void)
{
    if (!active || destroy)
        return;
    AddCollider(collider);
    Engine::GameObject::Update();
}
float delay = 0.0f;
void F15E::LateUpdate(void)
{
    if (!active || destroy)
        return;
    Engine::GameObject::LateUpdate();
    AddRenderObject(RenderType::NonBlend, this);
    if (std::isnan(transformComponent->Position().x))
        int i = 0;
}

void F15E::FixedUpdate(void)
{
    if (!active || destroy)
        return;
    Engine::GameObject::FixedUpdate();
}

void F15E::Render(void)
{
    if (!active || destroy)
        return;
    transformComponent->Render();
    model->Render();
    gearModel->Render();
    collider->Render();
}


