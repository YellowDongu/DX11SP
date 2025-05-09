#include "pch.h"
#include "Missile.h"
#include "RMWR.h"
#include "Explosion.h"

Explosion* Missile::explosion = nullptr;

Missile::Missile(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext), model(nullptr), target(nullptr), rotationSpeed(Vector2::one()), speed(4000.0f)
{
}

Missile::Missile(const Missile& other) : Engine::GameObject(other), model(nullptr), target(nullptr), rotationSpeed(other.rotationSpeed), speed(other.speed)
{
	model = static_cast<Engine::StaticModel*>(GetComponent(L"StaticModel"));
}

HRESULT Missile::Awake(void)
{
	return S_OK;
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

	if (angleDifference.x > 180.0f) angleDifference.x -= 360.0f;
	else if (angleDifference.x < -180.0f) angleDifference.x += 360.0f;

	if (angleDifference.y > 180.0f) angleDifference.y -= 360.0f;
	else if (angleDifference.y < -180.0f) angleDifference.y += 360.0f;


	if (angleDifference.x != 0.0f)
		angleDifference.x = std::copysign(rotationSpeed.x, angleDifference.x);

	if (angleDifference.y != 0.0f)
		angleDifference.y = std::copysign(rotationSpeed.y, angleDifference.y);

	angleDifference.z = 0.0f;

	transformComponent->RelativeRotate(angleDifference * 0.01f);
}

void Missile::Detonate(void)
{
	detonated = true;
	RMWR* rmwr = static_cast<RMWR*>(target->GetComponent(L"RMWR"));
	if(rmwr != nullptr)
		rmwr->HitMe(damage);

	if (explosion == nullptr)
		explosion = static_cast<Explosion*>(EngineInstance()->SceneManager()->CurrentScene()->FindLayer(L"ParticleLayer")->GetGameObject(L"Explosion"));

	explosion->AddExplosion(transformComponent->Position(), Vector3{ explosionScale, explosionScale, 0.0f });

	return;
}
