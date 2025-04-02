#include "pch.h"
#include "Player.h"
#include "FlightMovement.h"
#include "ModelLoader.h"
#include "MainCamera.h"

#include "DebugConsole.h"
#include "ModelConverter.h"

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

//#define f16c
//#define assimpTest

#ifdef f16c
#include "F16CHandler.h"
#else
#include "F15EHandler.h"
#endif



#define aircraftGlobalXMMatrix() DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f))
#define aircraftGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
#define gearGlobalMatrix(matrix) DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)))
#define convertAircraft(loadPath, savePath) ModelConverter testConvert;Matrix matrix;aircraftGlobalMatrix(matrix);testConvert.ConvertFBX(loadPath, savePath, matrix);
#define convertGear(loadPath, savePath) ModelConverter testConvert;Matrix matrix;gearGlobalMatrix(matrix);testConvert.ConvertFBX(loadPath, savePath, matrix);
#define loadWithAssimp(path) model = Engine::Model::Create(dxDevice, dxDeviceContext, path);model->SetGlobalOffset(aircraftGlobalXMMatrix());


HRESULT Player::Start(void)
{
    //convertAircraft("D:\\PJ\\C++\\Class\\JusinDx3D\\JusinDx3DClient\\Bin\\f15e.fbx", L"D:\\PJ\\C++\\Class\\JusinDx3D\\JusinDx3DClient\\Bin\\f15e.model");

    CreateTransform();
    transformComponent->Position() = Vector3(1643.0f, 2.0f, 405.0f);
    
    //convertGear("../Bin/f16c/f16cGear.FBX", L"../Bin/f16c/f16cGear.model");
    //if (FAILED(LoadModel(L"../Bin/f15eGear.model", gearModel)))
    //    return E_FAIL;
    if (FAILED(LoadModel(L"../Bin/f15eGear.model", gearModel)))
        return E_FAIL;
    // model setting
    #ifdef f16c
    metaData = F16CMetaData();
    transformComponent->Scale() = Vector3::one();
    #ifdef assimpTest
    loadWithAssimp(metaData.modelFilePath);
    #else
    if (FAILED(LoadModel(metaData.modelFilePath, model)))
        return E_FAIL;
    #endif
    boneHandler = F16CHandler::Create(dxDevice, dxDeviceContext, model, gearModel);
    #else
    metaData = F15EMetaData();
    #ifdef assimpTest
    loadWithAssimp(metaData.modelFilePath);
    #else
    if (FAILED(LoadModel(metaData.modelFilePath, model)))
        return E_FAIL;
    #endif
    boneHandler = F15EHandler::Create(dxDevice, dxDeviceContext, model, gearModel);
    #endif
    boneHandler->LinkModels(model, gearModel);
    gearModel->SetAnimation(0);
    model->SetAnimation(0);
    AddComponent(gearModel, L"GearModel");
    AddComponent(model, L"FullModel");
    //boneHandler = static_cast<Engine::BoneHandler*>(::GetComponent(metaData.handlerName));
    AddComponent(boneHandler, L"AircraftBoneHandler");
    cameraTrdViewOffset = metaData.cameraTrdViewOffset;
    cameraFstViewOffset = metaData.cameraFstViewOffset;
    // model setting

    flightModule = FlightMovement::Create(dxDevice, dxDeviceContext, transformComponent, metaData.flightSpec);
    AddComponent(flightModule, L"FlightMovement");
    static_cast<AircraftBoneHandler*>(boneHandler)->LinkYoke(flightModule->yoke);
    cameraState = 0;
    return S_OK;
}

HRESULT Player::Awake(void)
{
    if (FAILED(LoadModel(metaData.modelFilePath, model)))
        return E_FAIL;


    return S_OK;
}

void Player::Update(void)
{
    boneHandler->Update();

    if (Input()->getButtonDown(KeyType::R))
    {
        cameraState++;
    }

    if (camera == nullptr)
        return;


    switch (cameraState)
    {
    case -1:
    {
        camera->FOV(45.0f);
        if (Input()->getButton(KeyType::W))
        {
            if(Input()->getButton(KeyType::LShift))
                camera->transform()->Translate(camera->transform()->Forward());
            else
                camera->transform()->Translate(camera->transform()->Forward() * 0.05f);
        }
        if (Input()->getButton(KeyType::S))
        {
            if (Input()->getButton(KeyType::LShift))
                camera->transform()->Translate(camera->transform()->Forward() * -0.05f);
            else
                camera->transform()->Translate(camera->transform()->Forward() * -0.05f);
        }

        if (Input()->getButton(KeyType::A))
        {
            if (Input()->getButton(KeyType::LShift))
                camera->transform()->Translate(camera->transform()->Right() * -0.05f);
            else
                camera->transform()->Translate(camera->transform()->Right() * -0.05f);
        }
        if (Input()->getButton(KeyType::D))
        {
            if (Input()->getButton(KeyType::LShift))
                camera->transform()->Translate(camera->transform()->Right());
            else
                camera->transform()->Translate(camera->transform()->Right() * 0.05f);
        }


        if (Input()->getButton(KeyType::Q))
        {
            camera->transform()->Rotate({ 0.0f, 0.0f, 1.0f });
        }
        if (Input()->getButton(KeyType::E))
        {
            camera->transform()->Rotate({ 0.0f, 0.0f, -1.0f });
        }



        if (Input()->getButton(KeyType::I))
        {
            camera->transform()->RelativeRotate({ 0.1f, 0.0f, 0.0f });
            //transformComponent->Rotate({ 1.0f, 0.0f, 0.0f });
        }
        if (Input()->getButton(KeyType::K))
        {
            camera->transform()->RelativeRotate({ -0.1f, 0.0f, 0.0f });
            //transformComponent->Rotate({ -1.0f, 0.0f, 0.0f });
        }

        if (Input()->getButton(KeyType::J))
        {
            camera->transform()->Rotate({ 0.0f, -1.0f, 0.0f });
        }
        if (Input()->getButton(KeyType::L))
        {
            camera->transform()->Rotate({ 0.0f, 1.0f, 0.0f });
        }

        break;
    }
    case 0:
    {
        camera->FOV(45.0f);
        if (Input()->getButton(KeyType::W))
            flightModule->throttle = 1.0f;
        else if (Input()->getButton(KeyType::S))
            flightModule->throttle = 0.0f;
        else
            flightModule->ThrottleIdle();


        flightModule->yoke.x = static_cast<FLOAT>(Input()->getButton(KeyType::UP)) - static_cast<FLOAT>(Input()->getButton(KeyType::DOWN));
        flightModule->yoke.y = static_cast<FLOAT>(Input()->getButton(KeyType::A)) - static_cast<FLOAT>(Input()->getButton(KeyType::D));
        flightModule->yoke.z = static_cast<FLOAT>(Input()->getButton(KeyType::LEFT)) - static_cast<FLOAT>(Input()->getButton(KeyType::RIGHT));
        flightModule->Update();
        break;
    }
    case 1:
    {
        camera->FOV(0.3f);
        if (Input()->getButton(KeyType::W))
            flightModule->throttle = 1.0f;
        else if (Input()->getButton(KeyType::S))
            flightModule->throttle = 0.0f;
        else
            flightModule->ThrottleIdle();


        flightModule->yoke.x = static_cast<FLOAT>(Input()->getButton(KeyType::UP)) - static_cast<FLOAT>(Input()->getButton(KeyType::DOWN));
        flightModule->yoke.y = static_cast<FLOAT>(Input()->getButton(KeyType::A)) - static_cast<FLOAT>(Input()->getButton(KeyType::D));
        flightModule->yoke.z = static_cast<FLOAT>(Input()->getButton(KeyType::LEFT)) - static_cast<FLOAT>(Input()->getButton(KeyType::RIGHT));
        flightModule->Update();
        break;
    }
    default:
        cameraState = -1;
        break;
    }
}

void Player::LateUpdate(void)
{
    Engine::GameObject::LateUpdate();
    switch (cameraState)
    {
    case -1:
        break;
    case 0:
    {
        Vector3 position = transformComponent->Forward() * cameraTrdViewOffset.z + transformComponent->Up() * cameraTrdViewOffset.y;
        camera->transform()->Position() = transformComponent->Position() + position;
        camera->transform()->SetAngle(transformComponent->Quaternion());
        break;
    }
    case 1:
    {
        Vector3 position = transformComponent->Forward() * cameraFstViewOffset.z + transformComponent->Up() * cameraFstViewOffset.y;
        camera->transform()->Position() = transformComponent->Position() + position;
        camera->transform()->SetAngle(transformComponent->Quaternion());
        break;
    }
    default:
        cameraState = -1;
        break;
    }
    AddRenderObject(RenderType::NonBlend, this);
}

void Player::FixedUpdate(void)
{
}

void Player::Render(void)
{
    transformComponent->Render();
    model->Render();
    gearModel->Render();
}
