#include "pch.h"
#include "PlayerPilot.h"

#include "FlightMovement.h"
#include "MainCamera.h"
#include "FireControlSystem.h"
#include "StandardMissile.h"
#include "ActiveRadarHomingMissile.h"
#include "AircraftMetaData.h"
#include "RMWR.h"

PlayerPilot::PlayerPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::Component(dxDevice, dxDeviceContext)
{
}

PlayerPilot::PlayerPilot(const Component& other) : Engine::Component(other)
{
}

void PlayerPilot::Free(void)
{
}

PlayerPilot* PlayerPilot::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& objectInfomation)
{
	PlayerPilot* newInstance = new PlayerPilot(dxDevice, dxDeviceContext);
    newInstance->objectInfomation = objectInfomation;
	//if (FAILED(newInstance->Start()))
	//{
	//	Base::Destroy(newInstance);
	//	return nullptr;
	//}
	return newInstance;
}

Engine::Component* PlayerPilot::Clone(void)
{
	return new PlayerPilot(*this);
}

HRESULT PlayerPilot::Awake(void)
{
    transformComponent = gameObject->transform();
    flightModule = static_cast<FlightMovement*>(gameObject->GetComponent(L"FlightMovement"));
    fcs = static_cast<FireControlSystem*>(gameObject->GetComponent(L"FCS"));
    rmwr = static_cast<RMWR*>(gameObject->GetComponent(L"RMWR"));
    //cameraTrdViewOffset = metaData->cameraTrdViewOffset * 50.0f;
    //cameraFstViewOffset = metaData->cameraFstViewOffset * 50.0f;
    cameraTrdViewOffset = objectInfomation.aircraftInfomation.cameraTrdViewOffset * 0.5f;
    cameraFstViewOffset = objectInfomation.aircraftInfomation.cameraFstViewOffset * 0.5f;

    cameraState = -2;

    return S_OK;
}
FMOD::Channel* warningChannel = nullptr;
void PlayerPilot::Update(void)
{
    testEnemy = *testEnemyPointer;
    rmwr->Update();
    if (Input()->getButtonDown(KeyType::Space))
         fcs->weaponRelease = true;
    fcs->gunFire = Input()->getButton(KeyType::LCtrl);
    if (Input()->getButtonDown(KeyType::V))
    {
        cameraState++;
        switch (cameraState)
        {
        case 1:
            camera->FOV(45.0f);
            break;
        case 3:
            camera->FOV(45.0f);
            cameraState = -1;
            break;
        default:
            camera->FOV(45.0f);
            break;
        }
    }
    if (Input()->getButtonDown(KeyType::E))
        fcs->ChangeWeapon();

    if (camera == nullptr)
        return;


    fcs->Update();

    switch (cameraState)
    {
    case -2:
        cameraState = 0;
        break;
    case -1:
    {
        cameraState = 0;
        break;
        camera->FOV(45.0f);
        if (Input()->getButton(KeyType::W))
        {
            if (Input()->getButton(KeyType::LShift))
                camera->transform()->Translate(camera->transform()->Forward());
            else
                camera->transform()->Translate(camera->transform()->Forward() * 0.05f);
        }
        if (Input()->getButton(KeyType::S))
        {
            if (Input()->getButton(KeyType::LShift))
                camera->transform()->Translate(camera->transform()->Forward() * -1.0f);
            else
                camera->transform()->Translate(camera->transform()->Forward() * -0.05f);
        }

        if (Input()->getButton(KeyType::A))
        {
            if (Input()->getButton(KeyType::LShift))
                camera->transform()->Translate(camera->transform()->Right() * -1.0f);
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
            camera->transform()->RelativeRotate({ 0.0f, 0.0f, 1.0f });
        }
        if (Input()->getButton(KeyType::E))
        {
            camera->transform()->RelativeRotate({ 0.0f, 0.0f, -1.0f });
        }



        if (Input()->getButton(KeyType::I))
        {
            camera->transform()->RelativeRotate({ 0.1f, 0.0f, 0.0f });
        }
        if (Input()->getButton(KeyType::K))
        {
            camera->transform()->RelativeRotate({ -0.1f, 0.0f, 0.0f });
        }

        if (Input()->getButton(KeyType::J))
        {
            camera->transform()->RelativeRotate({ 0.0f, -0.01f, 0.0f });
        }
        if (Input()->getButton(KeyType::L))
        {
            camera->transform()->RelativeRotate({ 0.0f, 0.01f, 0.0f });
        }

    }
        break;
    case 0:
    {
        PlayerControl();
    }
        break;
    case 1:
    {
        PlayerControl();
    }
        break;
    case 2:
    {
        PlayerControl();
    }
        break;
    default:
        cameraState = 0;
        break;
    {
        camera->transform()->Synchronization(*transformComponent);
        Vector3 position = transformComponent->Forward() * cameraTrdViewOffset.z + transformComponent->Up() * cameraTrdViewOffset.y;
        camera->transform()->Position() += position;

        xmMatrix transform = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        camera->transform()->WorldMatrix(DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&transformComponent->WorldMatrix()), transform));

        cameraState = -1;
    }
        break;
    }
	// get input
}

float roundByFloatPrecision(float value, int steps)
{
    float unit = FLT_EPSILON * steps;
    return std::round(value / unit) * unit;
}

void PlayerPilot::LateUpdate(void)
{
    if (flightModule->throttle > flightModule->IdleThrottle())
        accelValue += DeltaTime() * 0.1f;
    else if(flightModule->throttle < flightModule->IdleThrottle())
        accelValue -= DeltaTime() * 0.1f;
    else
    {
        if (accelValue > 0.61f)
            accelValue -= DeltaTime() * 0.1f;
        else if (accelValue < 0.59f)
            accelValue += DeltaTime() * 0.1f;
        else
            accelValue = 0.6f;
    }
    accelValue = clamp(accelValue, 0.5f, 1.0f);

    cameraTrdViewOffset = objectInfomation.aircraftInfomation.cameraTrdViewOffset * (0.5f + (accelValue - 0.5f) * 0.5f);

    switch (cameraState)
    {
    case -1:
        camera->transform()->UpdateWorldMatrix();
        break;
    case 0:
    {
        camera->transform()->Synchronization(*transformComponent);
        Vector3 position = transformComponent->Forward() * cameraTrdViewOffset.z + transformComponent->Up() * cameraTrdViewOffset.y;
        camera->transform()->Position() += position;


        xmMatrix transform = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        camera->transform()->WorldMatrix(DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&transformComponent->WorldMatrix()), transform));
        break;
    }
    case 1:
    {
        camera->transform()->Synchronization(*transformComponent);
        Vector3 position = transformComponent->Forward() * cameraFstViewOffset.z + transformComponent->Up() * cameraFstViewOffset.y;
        camera->transform()->Position() += position;


        xmMatrix transform = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        camera->transform()->WorldMatrix(DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&transformComponent->WorldMatrix()), transform));
        break;
    }
    case 2:
    {
        camera->transform()->Synchronization(*transformComponent);
        Vector3 position = transformComponent->Forward() * cameraFstViewOffset.z + transformComponent->Up() * cameraFstViewOffset.y;
        camera->transform()->Position() += position;


        xmMatrix transform = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        camera->transform()->WorldMatrix(DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&transformComponent->WorldMatrix()), transform));
        break;
    }
    default:
    {
        camera->transform()->Synchronization(*transformComponent);
        Vector3 position = transformComponent->Forward() * cameraTrdViewOffset.z + transformComponent->Up() * cameraTrdViewOffset.y;
        camera->transform()->Position() += position;

        xmMatrix transform = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        camera->transform()->WorldMatrix(DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&transformComponent->WorldMatrix()), transform));
        cameraState = -1;

    }
        break;
    }


    /*
    
{
    if (flightModule->throttle > flightModule->IdleThrottle())
        accelValue += DeltaTime() * 0.1f;
    else if(flightModule->throttle < flightModule->IdleThrottle())
        accelValue -= DeltaTime() * 0.1f;
    else
    {
        if (accelValue > 0.61f)
            accelValue -= DeltaTime() * 0.1f;
        else if (accelValue < 0.59f)
            accelValue += DeltaTime() * 0.1f;
        else
            accelValue = 0.6f;
    }
    accelValue = clamp(accelValue, 0.5f, 1.0f);

    cameraTrdViewOffset = objectInfomation.aircraftInfomation.cameraTrdViewOffset * (0.5f + (accelValue - 0.5f) * 0.5f);

    switch (cameraState)
    {
    case -1:
        camera->transform()->UpdateWorldMatrix();
        break;
    case 0:
    {
        camera->transform()->Synchronization(*transformComponent);
        Vector3 position = transformComponent->Forward() * cameraTrdViewOffset.z + transformComponent->Up() * cameraTrdViewOffset.y;
        camera->transform()->Position() += position;


        xmMatrix transform = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        camera->transform()->WorldMatrix(DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&transformComponent->WorldMatrix()), transform));
        break;
    }
    case 1:
    {
        camera->transform()->Synchronization(*transformComponent);
        Vector3 position = transformComponent->Forward() * cameraFstViewOffset.z + transformComponent->Up() * cameraFstViewOffset.y;
        camera->transform()->Position() += position;


        xmMatrix transform = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        camera->transform()->WorldMatrix(DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&transformComponent->WorldMatrix()), transform));
        break;
    }
    case 2:
    {
        if (testEnemy == nullptr)
        {
            cameraState = -1;
            break;
        }
        
        camera->transform()->Synchronization(*testEnemy->transform());
        camera->transform()->Position() += testEnemy->transform()->Forward() * cameraTrdViewOffset.z + testEnemy->transform()->Up() * cameraTrdViewOffset.y;
        camera->transform()->UpdateWorldMatrix();
        
        break;
        Missile* launchedMissile = fcs->GetLastMissile();
        if (launchedMissile == nullptr || launchedMissile->Detonated())
        {
            camera->transform()->Synchronization(*transformComponent);
            Vector3 position = transformComponent->Forward() * cameraTrdViewOffset.z + transformComponent->Up() * cameraTrdViewOffset.y;
            camera->transform()->Position() += position;

            xmMatrix transform = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
            camera->transform()->WorldMatrix(DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&transformComponent->WorldMatrix()), transform));
            cameraState = -1;
            break;
        }

        camera->transform()->Synchronization(*launchedMissile->transform());
        //Vector3 position = launchedMissile->transform()->Forward() * cameraFstViewOffset.z * 3.0f;
        camera->transform()->Position() += launchedMissile->transform()->Forward() * -100.0f + launchedMissile->transform()->Up() * 25.0f;
        if(fcs->WeaponSelectedStatus() == 0)
            camera->transform()->SetAngle(static_cast<StandardMissile*>(launchedMissile)->Rotation());
        else
            camera->transform()->SetAngle(static_cast<ARHM*>(launchedMissile)->Rotation());

        camera->transform()->UpdateWorldMatrix();
        break;
    }
    default:
    {
        camera->transform()->Synchronization(*transformComponent);
        Vector3 position = transformComponent->Forward() * cameraTrdViewOffset.z + transformComponent->Up() * cameraTrdViewOffset.y;
        camera->transform()->Position() += position;

        xmMatrix transform = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        camera->transform()->WorldMatrix(DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&transformComponent->WorldMatrix()), transform));
        cameraState = -1;

    }
        break;
    }
    
    */
}

void PlayerPilot::FixedUpdate(void)
{
	//if (flightModule == nullptr)
	//	return;
}

void PlayerPilot::PlayerControl(void)
{
    bool accel = Input()->getButton(KeyType::W);
    bool deccel = Input()->getButton(KeyType::S);

    flightModule->highGTurn = accel && deccel;
    if (flightModule->highGTurn)
        flightModule->ThrottleIdle();
    else if (accel)
        flightModule->throttle = 1.0f;
    else if (deccel)
        flightModule->throttle = 0.0f;
    else
        flightModule->ThrottleIdle();


    flightModule->yoke.x = static_cast<FLOAT>(Input()->getButton(KeyType::UP)) - static_cast<FLOAT>(Input()->getButton(KeyType::DOWN));
    flightModule->yoke.y = static_cast<FLOAT>(Input()->getButton(KeyType::A)) - static_cast<FLOAT>(Input()->getButton(KeyType::D));
    flightModule->yoke.z = static_cast<FLOAT>(Input()->getButton(KeyType::LEFT)) - static_cast<FLOAT>(Input()->getButton(KeyType::RIGHT));
    flightModule->airbreakActive = deccel;
    flightModule->Update();
}
