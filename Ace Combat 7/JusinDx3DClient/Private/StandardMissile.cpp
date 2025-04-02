#include "pch.h"
#include "StandardMissile.h"

StandardMissile::StandardMissile(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

StandardMissile::StandardMissile(const StandardMissile& other) : Engine::GameObject(other)
{
}

void StandardMissile::Free(void)
{
	Engine::GameObject::Free();
}

StandardMissile* StandardMissile::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	StandardMissile* newInstance = new StandardMissile(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* StandardMissile::Clone(void)
{
	return new StandardMissile(*this);
}

HRESULT StandardMissile::Start(void)
{
	HRESULT result = CreateTransform();
	if (FAILED(result))
		return result;




	return S_OK;
}

void StandardMissile::Update(void)
{
	lifeTime += DeltaTime();
	if (lifeTime >= maximumLifeTime)
		/*delete this;*/ return;

	Chase();
}

void StandardMissile::LateUpdate(void)
{
}

void StandardMissile::FixedUpdate(void)
{
}

void StandardMissile::Render(void)
{
}

void StandardMissile::Chase(void)
{
	Vector3 direction = target->transform()->Position() - transformComponent->Position();
	if (direction.magnitude() >= 2.0f)
		Detonate(); return;

	direction = direction.normalize();
	transformComponent->Translate(direction * DeltaTime() * speed);
}

void StandardMissile::Detonate(void)
{
	return;
}
