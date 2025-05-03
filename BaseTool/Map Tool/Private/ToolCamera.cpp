#include "pch.h"
#include "ToolCamera.h"


//, device(nullptr), hWnd(nullptr),
//fov(0.0f), aspectRatio(0.0f), nearSightRange(0.0f), maximumSightRange(0.0f),
//windowSize{ 0.0f, 0.0f }, up{ 0.0f,1.0f,0.0f }, at{ 1.0f,0.0f,0.0f }, directionX{ 0.0f,0.0f,0.0f }, directionY{ 0.0f,0.0f,0.0f }, directionZ{ 1.0f,0.0f,0.0f },
//viewMatrix{}, projectionMatrix{}

ToolCamera::ToolCamera(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext), camera(nullptr)
{
}

ToolCamera::ToolCamera(const ToolCamera& other) : Engine::GameObject(other), camera(nullptr)
{
	camera = static_cast<Engine::Camera*>(GetComponent(L"Camera"));
}

void ToolCamera::Free(void)
{
	Engine::GameObject::Free();
}

ToolCamera* ToolCamera::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	ToolCamera* newInstance = new ToolCamera(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* ToolCamera::Clone(void)
{
	return new ToolCamera(*this);
}

HRESULT ToolCamera::Start(void)
{
	CreateTransform();
	const D3D11_VIEWPORT& viewport = Device()->ViewPortInfomation();
	camera = Engine::Camera::Create(dxDevice, dxDeviceContext, 45.0f, (viewport.Width / viewport.Height), 0.01f, 50000.0f);
	AddComponent(camera, L"Camera");

	return S_OK;
}

void ToolCamera::Update(void)
{
	if (::Input()->getButton(KeyType::A))
	{
		transformComponent->Position() -= transformComponent->Right();
	}
	if (::Input()->getButton(KeyType::D))
	{
		transformComponent->Position() += transformComponent->Right();
	}
	if (::Input()->getButton(KeyType::W))
	{
		transformComponent->Position() += transformComponent->Forward();
	}
	if (::Input()->getButton(KeyType::S))
	{
		transformComponent->Position() -= transformComponent->Forward();
	}
	if (::Input()->getButton(KeyType::Space))
	{
		transformComponent->Position() += transformComponent->Up();
	}
	if (::Input()->getButton(KeyType::C))
	{
		transformComponent->Position() -= transformComponent->Up();
	}

	angle += Vector3{
							   static_cast<FLOAT>(::Input()->getButton(KeyType::UP)) - static_cast<FLOAT>(::Input()->getButton(KeyType::DOWN)),
								static_cast<FLOAT>(::Input()->getButton(KeyType::LEFT)) - static_cast<FLOAT>(::Input()->getButton(KeyType::RIGHT)),
							   0.0f } *DeltaTime() * 10.0f;


	transformComponent->SetAngle(angle);

	//transformComponent->RelativeRotate(Vector3{
	//						   static_cast<FLOAT>(::Input()->getButton(KeyType::UP)) - static_cast<FLOAT>(::Input()->getButton(KeyType::DOWN)),
	//							static_cast<FLOAT>(::Input()->getButton(KeyType::LEFT)) - static_cast<FLOAT>(::Input()->getButton(KeyType::RIGHT)),
	//						   0.0f } *DeltaTime());
	// 
	//float x = static_cast<FLOAT>(::Input()->getMouseMovement().x);
	//float y = static_cast<FLOAT>(::Input()->getMouseMovement().y);
	//if(x != 0)
	//	x = std::fabs(x) / x;
	//if (y != 0)
	//	y = std::fabs(y) / y;
	//transformComponent->Rotate(Vector3{ x, y, 0.0f });
	Engine::GameObject::Update();
	camera->LateUpdate();
}

void ToolCamera::LateUpdate(void)
{
	Engine::GameObject::LateUpdate();
	AddRenderObject(RenderType::Camera, this);
}

void ToolCamera::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void ToolCamera::Render(void)
{
	transformComponent->Render();
	camera->Render(transformComponent->WorldMatrix());
}
