#include "pch.h"
#include "Player.h"
#include "FlightMovement.h"
#include "ModelLoader.h"
#include "MainCamera.h"
#include "FireControlSystem.h"
#include "AutoPilot.h"
#include "PlayerPilot.h"
#include "RMWR.h"
#include "RaderSystem.h"

#include "ModelConverter.h"
#include "Collider.h"

#include "StandardMissile.h"
#include "ActiveRadarHomingMissile.h"
#include "PlayerEar.h"
#include "TerrainCollision.h"
#include "WingVaporTrail.h"
#include "SimpleBullet.h"
Engine::GameObject* testEnemy = nullptr;

Player::Player(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext), model(nullptr), boneHandler(nullptr), flightModule(nullptr)
{
}

Player::Player(const Player& other) : Engine::GameObject(other), model(nullptr), boneHandler(nullptr), flightModule(nullptr)
{
}


void Player::Free(void)
{
    GameObject::Free();
}

Player* Player::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
    Player* newInstance = new Player(dxDevice, dxDeviceContext);

    if (FAILED(newInstance->Start()))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }
    return newInstance;
}

Player* Player::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& metaData)
{
    Player* newInstance = new Player(dxDevice, dxDeviceContext);
    newInstance->objectInfomation = metaData;
    if (FAILED(newInstance->Start(newInstance->objectInfomation)))
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

//#define f14d
#define f16c
//#define su33

#include "F16CHandler.h"
#include "F15EHandler.h"
#include "F14DHandler.h"
#include "SU33Handler.h"
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
RaderSystem* raderSystem = nullptr;
TerrainCollision* terrainCollision = nullptr;
HRESULT Player::Start(ObjectInfomation& objectInfomation)
{
    std::wstring name = L"F16CHandler";
    ::AddPrefabComponent(name, F16CHandler::Create(dxDevice, dxDeviceContext));
    name = L"F14DHandler";
    ::AddPrefabComponent(name, F14DHandler::Create(dxDevice, dxDeviceContext));
    name = L"SU33Handler";
    ::AddPrefabComponent(name, SU33Handler::Create(dxDevice, dxDeviceContext));
    name = L"F15EHandler";
    ::AddPrefabComponent(name, F15EHandler::Create(dxDevice, dxDeviceContext));

    if (FAILED(CreateTransform()))
        return E_FAIL;

    //transformComponent->Position() = Vector3(1643.0f, 2.0f, 405.0f);
    transformComponent->Scale() = Vector3::one() * 0.5f;
    //transformComponent->SetAngle(Vector3{ 0.0f, -45.0f, 0.0f });
    //transformComponent->Scale() = Vector3::one() * 0.01f;


    AircraftMetaData& metaData = objectInfomation.aircraftInfomation;

    Matrix matrix;
    aircraftGlobalMatrix(matrix);

    ConvertModel(metaData.modelFilePathA, metaData.modelFilePath, matrix);
    gearGlobalMatrix(matrix);
    ConvertModel(metaData.gearModelfilePathA, metaData.gearModelfilePath, matrix);
    if (FAILED(::LoadModel(metaData.gearModelfilePath, gearModel)))
        return E_FAIL;
    if (FAILED(::LoadModel(metaData.modelFilePath, model)))
        return E_FAIL;

    AddComponent(gearModel, L"GearModel");
    AddComponent(model, L"FullModel");

    boneHandler = static_cast<AircraftBoneHandler*>(::InstantiateComponent(metaData.BoneHandlerName));
    AddComponent(boneHandler, L"AircraftBoneHandler");
    // model setting

    flightModule = FlightMovement::Create(dxDevice, dxDeviceContext, metaData.flightSpec);
    AddComponent(flightModule, L"FlightMovement");

    fcs = FireControlSystem::Create(dxDevice, dxDeviceContext, objectInfomation);
    Missile* missile = StandardMissile::Create(dxDevice, dxDeviceContext);
    missile->SetDetonationDistance(ConvertFeetToWorld(5.0f) * 2.5f);
    fcs->SetStandardMissile(static_cast<StandardMissile*>(missile));
    missile = ARHM::Create(dxDevice, dxDeviceContext);
    missile->SetDetonationDistance(ConvertFeetToWorld(5.0f) * 2.5f);
    fcs->SetUniqueMissile(missile);
    fcs->GetBulletOriginal()->SetDamage(10.0f);
    AddComponent(fcs, L"FCS");

    Engine::OBB::Description description = Engine::OBB::Description();
    description.center = metaData.colliderCenter;
    description.extents = metaData.colliderExtent;
    DirectX::XMStoreFloat4(&description.quaternion, DirectX::XMQuaternionIdentity());

    collider = Engine::Collider::Create(dxDevice, dxDeviceContext, &description);
    if (collider == nullptr)
        return E_FAIL;
    AddComponent(collider, L"Collider");

    AddComponent(comTest = RMWR::Create(dxDevice, dxDeviceContext), L"RMWR");
    comTest->SetInvincibility(true);
    comTest->SetMAXHealth(100.0f);
    playerPilot = PlayerPilot::Create(dxDevice, dxDeviceContext, objectInfomation);
    AddComponent(playerPilot, L"PlayerPilot");
    playerPilot->testEnemyPointer = &testEnemy;
    AddComponent(comTest2 = PlayerEar::Create(dxDevice, dxDeviceContext, metaData), L"PlayerEar");

    Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "Weapons.wav");
    terrainCollision = TerrainCollision::Create(dxDevice, dxDeviceContext);
    AddComponent(terrainCollision, L"TerrainCollision");

    raderSystem = RaderSystem::Create(dxDevice, dxDeviceContext);
    AddComponent(raderSystem, L"RaderSystem");
    return S_OK;
}
HRESULT Player::Start(void)
{
    std::wstring name = L"F16CHandler";
    ::AddPrefabComponent(name, F16CHandler::Create(dxDevice, dxDeviceContext));
    name = L"F14DHandler";
    ::AddPrefabComponent(name, F14DHandler::Create(dxDevice, dxDeviceContext));
    name = L"SU33Handler";
    ::AddPrefabComponent(name, SU33Handler::Create(dxDevice, dxDeviceContext));
    name = L"F15EHandler";
    ::AddPrefabComponent(name, F15EHandler::Create(dxDevice, dxDeviceContext));

    if (FAILED(CreateTransform()))
        return E_FAIL;

    transformComponent->Scale() = Vector3::one() * 0.5f;
    //transformComponent->SetAngle(Vector3{ 0.0f, -45.0f, 0.0f });
    //transformComponent->Scale() = Vector3::one() * 0.01f;
    
    // model setting
    #if defined(f16c)
    objectInfomation.aircraftInfomation = F16CMetaData();
    #elif defined(f14d)
    objectInfomation.aircraftInfomation = F14DMetaData();
    #elif defined(su33)
    objectInfomation.aircraftInfomation = SU33MetaData();
    #else
    objectInfomation.aircraftInfomation = F15EMetaData();
    #endif
    AircraftMetaData& metaData = objectInfomation.aircraftInfomation;

    Matrix matrix;
    aircraftGlobalMatrix(matrix);

    ConvertModel(metaData.modelFilePathA, metaData.modelFilePath, matrix);
    gearGlobalMatrix(matrix);
    ConvertModel(metaData.gearModelfilePathA, metaData.gearModelfilePath, matrix);
    if (FAILED(::LoadModel(metaData.gearModelfilePath, gearModel)))
        return E_FAIL;
    if (FAILED(::LoadModel(metaData.modelFilePath, model)))
        return E_FAIL;


    boneHandler = static_cast<AircraftBoneHandler*>(::InstantiateComponent(metaData.BoneHandlerName));
    if (boneHandler == nullptr)
    {
        #if defined(f16c)
        boneHandler = F16CHandler::Create(dxDevice, dxDeviceContext);
        #elif defined(f14d)
        boneHandler = F14DHandler::Create(dxDevice, dxDeviceContext);
        #elif defined(su33)
        boneHandler = SU33Handler::Create(dxDevice, dxDeviceContext);
        #else
        boneHandler = F15EHandler::Create(dxDevice, dxDeviceContext);
        #endif
    }

    AddComponent(gearModel, L"GearModel");
    AddComponent(model, L"FullModel");

    //boneHandler = static_cast<Engine::BoneHandler*>(::GetComponent(metaData.handlerName));
    AddComponent(boneHandler, L"AircraftBoneHandler");
    // model setting

    flightModule = FlightMovement::Create(dxDevice, dxDeviceContext, metaData.flightSpec);
    AddComponent(flightModule, L"FlightMovement");

    fcs = FireControlSystem::Create(dxDevice, dxDeviceContext, objectInfomation);
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

    AddComponent(comTest = RMWR::Create(dxDevice, dxDeviceContext), L"RMWR");
    comTest->SetInvincibility(true);
    comTest->SetMAXHealth(100.0f);
    playerPilot = PlayerPilot::Create(dxDevice, dxDeviceContext, objectInfomation);
    AddComponent(playerPilot, L"PlayerPilot");
    playerPilot->testEnemyPointer = &testEnemy;
    AddComponent(comTest2 = PlayerEar::Create(dxDevice, dxDeviceContext, metaData), L"PlayerEar");

    Sound()->LoadSound("../Bin/Resources/Sounds/Effects/", "Weapons.wav");
    terrainCollision = TerrainCollision::Create(dxDevice, dxDeviceContext);
    AddComponent(terrainCollision, L"TerrainCollision");

    raderSystem = RaderSystem::Create(dxDevice, dxDeviceContext);
    raderSystem->SetMaxTimer(1.5f);
    AddComponent(raderSystem, L"RaderSystem");

    return S_OK;
}
HRESULT Player::Awake(void)
{
    boneHandler->Awake();
    flightModule->Awake();
    static_cast<AircraftBoneHandler*>(boneHandler)->LinkYoke(flightModule->yoke);
    playerPilot->Awake();
    comTest2->Awake();
    terrainCollision->Awake();
    raderSystem->Awake();
    cameraState = 0;

    Engine::Layer* layer = ::EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"ParticleLayer");
    if (layer == nullptr)
        return E_FAIL;

    WingVaporTrail* wingVaporParticle = static_cast<WingVaporTrail*>(layer->GetGameObject(L"WingVaporTrail"));
    if (wingVaporParticle == nullptr)
        return E_FAIL;

    wingVaporParticle->EnlistGameObject(this);
    //for (auto& component : components)
    //    component.second->Awake();

    return S_OK;
}

void Player::Update(void)
{
    AddCollider(collider);
    std::cout << DeltaTime() << "   : " << Time()->FPS() << std::endl;
    if (testEnemy == nullptr)
    {
        //testEnemy = EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"MainTargetEnemy")->GetGameObject(L"MainTarget");
        testEnemy = EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"Ally")->GetGameObject(L"WingMan");
    }
    boneHandler->Update();
    playerPilot->Update();
    comTest2->Update();
    collider->Update();
    raderSystem->Update();
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
    Engine::GameObject::FixedUpdate();
}

void Player::Render(void)
{
    if (playerPilot->CameraState() == 1)
    {
        camera->FOV(7.5f);
        const Engine::PipelineStatus& pipeLine = ::GetPipeLineStatus();
        Matrix viewMatrix, identity;
        memcpy(&viewMatrix, &pipeLine.viewMatrix, sizeof(Matrix));
        DirectX::XMStoreFloat4x4(&identity, DirectX::XMMatrixIdentity());

        ::SetViewProjectionMatrix(identity, pipeLine.projectionMatrix);
        Vector3 firstOffset = playerPilot->LinkObjectInfomation().aircraftInfomation.cameraFstViewOffset * -1.0f;
        identity._41 = firstOffset.x;
        identity._42 = firstOffset.y;
        identity._43 = firstOffset.z;

        ::SetWorldMatrix(identity);
        model->Render();

        camera->FOV(45.0f);
        ::SetViewProjectionMatrix(viewMatrix, pipeLine.projectionMatrix);
        transformComponent->Render();
    }
    else if (playerPilot->CameraState() == 0)
    {
        transformComponent->Render();
        model->Render();
        gearModel->Render();
    }

    collider->Render();
}


void Player::LinkCamera(MainCamera* object) { camera = object; if (playerPilot != nullptr) playerPilot->LinkCamera(object); }

void Player::Collide(Engine::GameObject* target, Engine::Collider* targetCollision)
{
    int alarm = 0;
}
