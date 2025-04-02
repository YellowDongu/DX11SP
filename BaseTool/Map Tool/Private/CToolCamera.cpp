#include "pch.h"
#include "CToolCamera.h"


ToolCamera::ToolCamera(void) : transformComponent(nullptr), device(nullptr), hWnd(nullptr),
fov(0.0f), aspectRatio(0.0f), nearSightRange(0.0f), maximumSightRange(0.0f),
windowSize{ 0.0f, 0.0f }, up{ 0.0f,1.0f,0.0f }, at{ 1.0f,0.0f,0.0f }, directionX{ 0.0f,0.0f,0.0f }, directionY{ 0.0f,0.0f,0.0f }, directionZ{ 1.0f,0.0f,0.0f },
viewMatrix{}, projectionMatrix{}
{
}

ToolCamera::~ToolCamera(void)
{
}

_int ToolCamera::Update_Component(const _float& fTimeDelta)
{
	Get_Window_Infomation();
	if (transformComponent)
	{
		directionZ = direction(transformComponent->Get_Angle());
		currentPosition = transformComponent->Get_Position();
	}
	else
	{
		directionZ = direction({ 0.0f, 0.0f ,0.0f });
	}
	at = directionZ + currentPosition;
	directionX = cross(directionZ, up);
	directionY = cross(directionX, directionZ);
	return 0;
}

void ToolCamera::Set_Camera(void)
{
	D3DXMatrixLookAtLH(&viewMatrix, &currentPosition, &at, &up);
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DXToRadian(fov), aspectRatio, nearSightRange, maximumSightRange);

	device->SetTransform(D3DTS_VIEW, &viewMatrix);
	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

void ToolCamera::SetUICamera(void)
{
	D3DXMATRIX UIViewMatrix;
	D3DXMatrixIdentity(&UIViewMatrix);
	device->SetTransform(D3DTS_VIEW, &UIViewMatrix);

	D3DXMatrixOrthoLH(&orthoMatrix, 1000, 1000, 0, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &orthoMatrix);
}

void ToolCamera::Get_Window_Infomation()
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	windowSize.x = static_cast<float>(clientRect.right - clientRect.left);
	windowSize.y = static_cast<float>(clientRect.bottom - clientRect.top);
	aspectRatio = windowSize.x / windowSize.y;
}

ToolCamera* ToolCamera::Create(HWND _hWnd, LPDIRECT3DDEVICE9& _device, CTransform* _transformComponent)
{
	ToolCamera* newCamera = new ToolCamera();
	newCamera->hWnd = _hWnd;
	newCamera->device = _device;
	newCamera->transformComponent = _transformComponent;
	newCamera->nearSightRange = 0.1f;
	newCamera->maximumSightRange = 1000.0f;
	newCamera->fov = 90.0f;
	return newCamera;
}

CComponent* ToolCamera::Clone()
{
	return nullptr;
}
