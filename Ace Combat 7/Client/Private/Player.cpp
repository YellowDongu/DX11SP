#include "pch.h"
#include "Player.h"
#include "FlightMovement.h"
#include "ModelLoader.h"
#include "MainCamera.h"
#include "FireControlSystem.h"
#include "AutoPilot.h"
#include "PlayerPilot.h"
#include "RMWR.h"

#include "ModelConverter.h"
#include "Collider.h"

#include "StandardMissile.h"
#include "ActiveRadarHomingMissile.h"
#include "PlayerEar.h"

#include "WingTrailParticle.h"
Engine::Transform* testtransform = nullptr;

Engine::GameObject* testEnemy = nullptr;
Player::Player(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext), model(nullptr), boneHandler(nullptr), flightModule(nullptr), metaData{}
{
}

Player::Player(const Player& other) : Engine::GameObject(other), model(nullptr), boneHandler(nullptr), flightModule(nullptr), metaData{}
{
}


void Player::Free(void)
{
    GameObject::Free();
}

Player* Player::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, AircraftMetaData* metaData)
{
    ::EngineInstance()->CreateConsole();
    Player* newInstance = new Player(dxDevice, dxDeviceContext);
    if (metaData != nullptr)
        newInstance->metaData = *metaData;
    if (FAILED(newInstance->Start()))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }
    return newInstance;
}

Engine::GameObject* Player::Clone(void)
{
    Player* newInstance = new Player(*this);
    if (FAILED(newInstance->Awake()))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }
    return newInstance;
}

#define f14d
//#define f16c
//#define su33

#include "F16CHandler.h"
#include "F15EHandler.h"
#include "F14DHandler.h"
#include "SU33Handler.h"
WingTrailParticle* womgTrail;
#ifdef useAssimp
//#define aircraftGlobalXMMatrix() DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f))
//#define aircraftGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
//#define gearGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))


#define aircraftGlobalXMMatrix() DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f))
#define aircraftGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
#define gearGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
#define convertAircraft(loadPath, savePath) ModelConverter testConvert;Matrix matrix;aircraftGlobalMatrix(matrix);testConvert.ConvertFBX(loadPath, savePath, matrix);
#define convertGear(loadPath, savePath) ModelConverter testConvert;Matrix matrix;gearGlobalMatrix(matrix);testConvert.ConvertFBX(loadPath, savePath, matrix);
#define loadWithAssimp(path) model = Engine::Model::Create(dxDevice, dxDeviceContext, path);model->SetGlobalOffset(aircraftGlobalXMMatrix());

#endif
RMWR* comTest = nullptr;
PlayerEar* comTest2 = nullptr;
HRESULT Player::Start(void)
{
    CreateTransform();
    transformComponent->Position() = Vector3(1643.0f, 2.0f, 405.0f);
    transformComponent->Scale() = Vector3::one();
    transformComponent->SetAngle(Vector3{ 0.0f, -45.0f, 0.0f });
    //transformComponent->Scale() = Vector3::one() * 0.01f;
    
    // model setting
    #if defined(f16c)
    metaData = F16CMetaData();
    #elif defined(f14d)
    metaData = F14DMetaData();
    #elif defined(su33)
    metaData = SU33MetaData();
    #else
    metaData = F15EMetaData();
    #endif


    Matrix matrix;
    aircraftGlobalMatrix(matrix);

    ConvertModel(metaData.modelFilePathA, metaData.modelFilePath, matrix);
    gearGlobalMatrix(matrix);
    ConvertModel(metaData.gearModelfilePathA, metaData.gearModelfilePath, matrix);
    if (FAILED(::LoadModel(metaData.gearModelfilePath, gearModel)))
        return E_FAIL;
    if (FAILED(::LoadModel(metaData.modelFilePath, model)))
        return E_FAIL;

    #if defined(f16c)
    boneHandler = F16CHandler::Create(dxDevice, dxDeviceContext);
    #elif defined(f14d)
    boneHandler = F14DHandler::Create(dxDevice, dxDeviceContext);
    #elif defined(su33)
    boneHandler = SU33Handler::Create(dxDevice, dxDeviceContext);
    #else
    boneHandler = F15EHandler::Create(dxDevice, dxDeviceContext);
    #endif

    boneHandler->LinkModels(model, gearModel);
    AddComponent(gearModel, L"GearModel");
    AddComponent(model, L"FullModel");

    //boneHandler = static_cast<Engine::BoneHandler*>(::GetComponent(metaData.handlerName));
    AddComponent(boneHandler, L"AircraftBoneHandler");
    boneHandler->Awake();
    // model setting

    flightModule = FlightMovement::Create(dxDevice, dxDeviceContext, transformComponent, metaData.flightSpec);
    AddComponent(flightModule, L"FlightMovement");
    static_cast<AircraftBoneHandler*>(boneHandler)->LinkYoke(flightModule->yoke);

    fcs = FireControlSystem::Create(dxDevice, dxDeviceContext, metaData);
    fcs->SetStandardMissile(StandardMissile::Create(dxDevice, dxDeviceContext));
    fcs->SetUniqueMissile(ARHM::Create(dxDevice, dxDeviceContext));
    AddComponent(fcs, L"FCS");

    Engine::OBB::Description description = Engine::OBB::Description();
    description.center = metaData.colliderCenter;
    description.extents = metaData.colliderExtent;
    DirectX::XMStoreFloat4(&description.quaternion, DirectX::XMQuaternionIdentity());

    collider = Engine::Collider::Create(dxDevice, dxDeviceContext, &description);
    if (collider == nullptr)
        return E_FAIL;
    AddComponent(collider, L"Collider");

    womgTrail = WingTrailParticle::Create(dxDevice, dxDeviceContext);
    AddComponent(womgTrail, L"WingTrailParticle");
    womgTrail->Awake();
    AddComponent(comTest = RMWR::Create(dxDevice, dxDeviceContext), L"RMWR");
    playerPilot = PlayerPilot::Create(dxDevice, dxDeviceContext, metaData);
    AddComponent(playerPilot, L"PlayerPilot");
    playerPilot->Awake();
    playerPilot->testEnemyPointer = &testEnemy;
    AddComponent(comTest2 = PlayerEar::Create(dxDevice, dxDeviceContext, metaData), L"PlayerEar");
    comTest2->Awake();
    cameraState = 0;

    Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "MissileFired.wav");
    return S_OK;
}
HRESULT Player::Awake(void)
{
    if (FAILED(LoadModel(metaData.modelFilePath, model)))
        return E_FAIL;

    for (auto& component : components)
        component.second->Awake();

    return S_OK;
}

void Player::Update(void)
{
    AddCollider(collider);
    std::cout << DeltaTime() << "   : " << Time()->FPS() << std::endl;
    if (testEnemy == nullptr)
    {
        testEnemy = EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"MainTargetEnemy")->GetGameObject(L"MainTargetEnemy");
    }
    boneHandler->Update();
    playerPilot->Update();
    comTest2->Update();
    collider->Update();
    return;
}

void Player::LateUpdate(void)
{
    AddRenderObject(RenderType::NonBlend, this);
    transformComponent->UpdateWorldMatrix();
    Engine::GameObject::LateUpdate();
}

void Player::FixedUpdate(void)
{
}

void Player::Render(void)
{
    transformComponent->Render();
    model->Render();
    gearModel->Render();
    collider->Render();
    womgTrail->Render();
}


void Player::LinkCamera(MainCamera* object) { camera = object; if (playerPilot != nullptr) playerPilot->LinkCamera(object); }

void Player::Collide(Engine::GameObject* target, Engine::Collider* targetCollision)
{
    int alarm = 0;
}
