#include "pch.h"
#include "Missile.h"
#include "RMWR.h"
Missile::Missile(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext), model(nullptr), target(nullptr), rotationSpeed(Vector2::one()), speed(4000.0f)
{
}

Missile::Missile(const Missile& other) : Engine::GameObject(other), model(nullptr), target(nullptr), rotationSpeed(other.rotationSpeed), speed(other.speed)
{
	model = static_cast<Engine::StaticModel*>(GetComponent(L"StaticModel"));
}

void Missile::Update(void)
{
}

void Missile::LateUpdate(void)
{
}

void Missile::FixedUpdate(void)
{
}

void Missile::Render(void)
{
}

void Missile::Rotation(const Vector3& destination)
{
	Vector3 angleDifference = Vector3::getDirection(destination.normalize()) - Vector3::getDirection(transformComponent->Forward());

	if(angleDifference.x != 0.0f)
		angleDifference.x = (angleDifference.x / std::abs(angleDifference.x)) * rotationSpeed.x;
	if (angleDifference.y != 0.0f)
		angleDifference.y = (angleDifference.y / std::abs(angleDifference.y)) * rotationSpeed.y;
	angleDifference.z = 0.0f;

	transformComponent->RelativeRotate(angleDifference * 0.01f);
}

void Missile::Detonate(void)
{
	detonated = true;
	RMWR* rmwr = static_cast<RMWR*>(target->GetComponent(L"RMWR"));
	if(rmwr != nullptr)
		rmwr->HitMe(damage);

	return;
}
