#include "pch.h"
#include "CMainViewCamera.h"




CMainViewCamera::CMainViewCamera() : CGameObject(nullptr), cameraComponent(nullptr), transformComponent(nullptr), cameraMoveSpeed(10.f), cameraRotateSpeed(30.f)
{
}

CMainViewCamera::CMainViewCamera(LPDIRECT3DDEVICE9& device) : CGameObject(device), cameraComponent(nullptr), transformComponent(nullptr), cameraMoveSpeed(10.f), cameraRotateSpeed(30.f)
{
}

CMainViewCamera::~CMainViewCamera()
{
}

void CMainViewCamera::Render_GameObject()
{
    cameraComponent->Set_Camera();
}

void CMainViewCamera::Render_UI(void)
{
    cameraComponent->SetUICamera();
}

_int CMainViewCamera::Update_GameObject(const _float& fTimeDelta)
{
    if (Input->getButton(KeyType::W))
    {
        transformComponent->Translate(transformComponent->Forward() * Time->deltaTime() * cameraMoveSpeed);
    }
    if (Input->getButton(KeyType::S))
    {
        transformComponent->Translate(transformComponent->Forward() * -Time->deltaTime() * cameraMoveSpeed);
    }

    if (Input->getButton(KeyType::D))
    {
        vec3 right, up = { 0.0f,1.0f,0.0f }, forward = transformComponent->Forward();
        D3DXVec3Cross(&right, &up, &forward);

        transformComponent->Translate(right * Time->deltaTime() * cameraMoveSpeed);
    }
    if (Input->getButton(KeyType::A))
    {
        vec3 right, up = { 0.0f,1.0f,0.0f }, forward = transformComponent->Forward();
        D3DXVec3Cross(&right, &up, &forward);

        transformComponent->Translate(right * -Time->deltaTime() * cameraMoveSpeed);
    }
    
    if (Input->getButton(KeyType::Space))
    {
        vec3 right, up = { 0.0f,1.0f,0.0f }, forward = transformComponent->Forward();
        D3DXVec3Cross(&right, &up, &forward);
        D3DXVec3Cross(&up, &forward, &right);

        transformComponent->Translate(up * Time->deltaTime() * cameraMoveSpeed);
    }
    if (Input->getButton(KeyType::C))
    {
        vec3 right, up = { 0.0f,1.0f,0.0f }, forward = transformComponent->Forward();
        D3DXVec3Cross(&right, &up, &forward);
        D3DXVec3Cross(&up, &forward, &right);


        transformComponent->Translate(up * -Time->deltaTime() * cameraMoveSpeed);
    }

    if (Input->getMouseButton(MouseKey::MButton))
    {
        vec3 CameraRotation = { (float)Input->getMouseMovement().y, (float)Input->getMouseMovement().x, 0.0f };
        CameraRotation *= Time->deltaTime() * cameraRotateSpeed;
        transformComponent->Rotation(CameraRotation);
        if (transformComponent->Angle().x > 85.0f)
            transformComponent->Angle().x = 85.0f;
        if (transformComponent->Angle().x < -85.0f)
            transformComponent->Angle().x = -85.0f;
    }

    cameraComponent->Update_Component(fTimeDelta);
    cameraMoveSpeed += Input->getMouseWheel() * Time->deltaTime();
    if (cameraMoveSpeed <= 0.0f)
        cameraMoveSpeed = 0.0f;
	return 0;
}

CMainViewCamera* CMainViewCamera::Create(LPDIRECT3DDEVICE9& device, HWND _hWnd)
{
	CMainViewCamera* newCamera = new CMainViewCamera(device);
	newCamera->transformComponent = CTransform::Create(device);
	newCamera->cameraComponent = ToolCamera::Create(_hWnd, device, newCamera->transformComponent);
	return newCamera;
}
